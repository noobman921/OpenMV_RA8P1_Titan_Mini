/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2020-2021 Damien P. George
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */
#include "tusb.h"

#if CFG_TUD_MSC

#include <string.h>
#include "mpconfigboard.h"
#include "drivers/fsp_sdcard.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef MICROPY_HW_USB_MSC_INQUIRY_VENDOR_STRING
#define MICROPY_HW_USB_MSC_INQUIRY_VENDOR_STRING "MicroPy"
#endif

#ifndef MICROPY_HW_USB_MSC_INQUIRY_PRODUCT_STRING
#define MICROPY_HW_USB_MSC_INQUIRY_PRODUCT_STRING "Mass Storage"
#endif

#ifndef MICROPY_HW_USB_MSC_INQUIRY_REVISION_STRING
#define MICROPY_HW_USB_MSC_INQUIRY_REVISION_STRING "1.0"
#endif

extern volatile bool sdcard_inserted;

static bool ejected = false;

/* ============================================================================
 * MSC 的 SD 访问：异步提交，在独立低优先级 worker 里做
 * ============================================================================
 * 结构对齐 Titan_Mini_Openmv/platform/usb/ra8_usb_msc.c。
 *
 * 为什么必须这样（本项目实际踩到的坑）：
 *   MSC 回调（tud_msc_read10_cb 等）是在 USB 任务里、由 tud_task_ext() 调用的。
 *   若在这里直接做 SD I/O（R_SDHI 忙等 DMA 完成），USB 任务的整条循环被卡住：
 *   tud_task 停摆 → EP0/CDC 全部无响应 → **Windows 约 10s 后复位端口** →
 *   DCD_EVENT_BUS_RESET → IDE 的 CDC 会话被杀 → 表现为「空闲一会儿就断连」。
 *   实测（tools/usb_probe 探针）：会话正是在总线复位时死亡（clrSrc=1）。
 *
 * 关键点：
 *   1. 回调只提交作业，立刻返回 TUD_MSC_RET_ASYNC；
 *   2. worker 优先级 2 —— **低于** MPY Thread(3) 与 USB svc 任务(4)，
 *      慢速介质等待永远不会阻塞 USB 或 VM；
 *   3. 但正因更低，VM 的忙循环会饿死它 → mpconfigport.h 的
 *      MICROPY_EVENT_POLL_HOOK 里加了让出点（对应 Titan
 *      platform/micropython/mphal.c 的 rt_thread_mdelay(1)）；
 *   4. 用私有 bounce buffer 做 I/O，绝不把 TinyUSB 的端点缓冲交给存储层 ——
 *      总线复位会立刻复用那个缓冲发新的 CBW；
 *   5. generation 计数器让「复位后到达的旧完成」作废，不会去 ack 新命令。
 * ==========================================================================*/

typedef enum {
    MSC_IO_IDLE = 0,
    MSC_IO_QUEUED,
    MSC_IO_RUNNING,
    MSC_IO_DONE,
} msc_io_state_t;

static struct {
    volatile msc_io_state_t state;
    uint32_t generation;
    uint32_t lba, offset, size;
    uint8_t lun;
    bool writing;
    void *usb_buffer;
    int32_t result;
} msc_io;

static volatile uint32_t msc_generation = 1;
static SemaphoreHandle_t msc_io_ready;
static bool msc_worker_started;
static TaskHandle_t msc_io_handle;

/* 存储层只用这块，绝不用 TinyUSB 的端点缓冲。
 * 32 字节对齐以配合 D-cache 维护。 */
static uint8_t msc_bounce[CFG_TUD_MSC_EP_BUFSIZE] __attribute__((aligned(32)));

/* 临界区：用 PRIMASK 保存/恢复，不用 taskENTER_CRITICAL()。
 *
 * ⚠️ 这里踩过一个致命坑（2026-09-22 实测定位）：
 * ra8_msc_reset() 是从 tud_event_hook_cb() 调用的，而那个钩子**在 USB 中断里**
 * 执行（dcd_int_handler → dcd_event_handler → queue_event → tud_event_hook_cb）。
 * FreeRTOS 的 vPortEnterCritical() 里有
 *     configASSERT( (portNVIC_INT_CTRL_REG & portVECTACTIVE_MASK) == 0 );
 * 而本工程 FreeRTOSConfig.h:168 是 configASSERT(x) = assert(x)
 *   → 断言失败 → __fatal_error() 死循环
 *   → 调度器再没跑过（xTickCount 恒 0）→ USB 从不枚举（"USB设备无法识别"）。
 * 当时的调用链（J-Link 抓栈）：
 *   Reset_Handler → main → vTaskStartScheduler → xPortStartScheduler
 *     → xCreateTask → 【USB ISR】dcd_int_handler → … → ra8_msc_reset
 *       → vPortEnterCritical → assert
 *
 * PRIMASK 保存/恢复在 ISR 与任务上下文都合法，也不会破坏 uxCriticalNesting。
 * 与 Titan 一致：它的 ra8_usb_msc_reset()/needs_service()/poll() 用的都是
 * rt_hw_interrupt_disable()（等价于 PRIMASK）。 */
static inline uint32_t msc_irq_save(void) {
    uint32_t primask = __get_PRIMASK();
    __disable_irq();
    return primask;
}

static inline void msc_irq_restore(uint32_t primask) {
    __set_PRIMASK(primask);
}

static int32_t msc_media_transfer(void) {
    const uint32_t sector_size = SDCard_GetSectorSize();
    const uint32_t blocks = msc_io.size / sector_size;
    int ret;
    if (msc_io.writing) {
        ret = SDCard_WriteBlocks(msc_io.lba, msc_bounce, blocks);
    } else {
        ret = SDCard_ReadBlocks(msc_io.lba, msc_bounce, blocks);
    }
    return (ret == 0) ? (int32_t) msc_io.size : -1;
}

static void ra8_msc_worker(void *arg) {
    (void) arg;
    for (;;) {
        if (xSemaphoreTake(msc_io_ready, portMAX_DELAY) != pdTRUE) {
            continue;
        }
        const uint32_t _irq = msc_irq_save();
        if (msc_io.state != MSC_IO_QUEUED) {
            msc_irq_restore(_irq);
            continue;
        }
        if (msc_io.generation != msc_generation) {
            msc_io.state = MSC_IO_DONE;
            msc_irq_restore(_irq);
            continue;
        }
        msc_io.state = MSC_IO_RUNNING;
        msc_irq_restore(_irq);

        /* 全部介质访问在这里完成：低于 USB 与 VM 的优先级，
         * 绝不在 USB 回调里等介质。 */
        const int32_t result = msc_media_transfer();

        const uint32_t _irq2 = msc_irq_save();
        msc_io.result = result;
        msc_io.state = MSC_IO_DONE;
        msc_irq_restore(_irq2);
    }
}

void ra8_msc_init(void) {
    if (msc_worker_started) {
        return;
    }
    msc_io_ready = xSemaphoreCreateBinary();
    if (msc_io_ready == NULL) {
        return;
    }
    /* 优先级 2：低于 MPY Thread(3) 与 svc(4)。必须低于 USB 任务，
     * 否则慢速介质会拖住 tud_task（见文件头说明）。 */
    if (xTaskCreate(ra8_msc_worker, "msc_io", 1024, NULL, 2, &msc_io_handle) != pdPASS) {
        vSemaphoreDelete(msc_io_ready);
        msc_io_ready = NULL;
        return;
    }
    msc_worker_started = true;
}

/* 总线复位 / BOT reset：作废在途作业，绝不回收正在进行的 I/O。
 * 由 tud_event_hook_cb 的 DCD_EVENT_BUS_RESET 调用。 */
void ra8_msc_reset(void) {
    const uint32_t _irq = msc_irq_save();
    msc_generation++;
    msc_irq_restore(_irq);
}

/* 让连续运行的 VM（REPL 忙循环）给低优先级 worker 一个 tick。
 * 由 mpconfigport.h 的 MICROPY_EVENT_POLL_HOOK 调用。 */
bool ra8_msc_needs_service(void) {
    const uint32_t _irq = msc_irq_save();
    const bool active = (msc_io.state == MSC_IO_QUEUED) || (msc_io.state == MSC_IO_RUNNING);
    msc_irq_restore(_irq);
    return active;
}

/* USB 任务在 tud_task_ext() 之后调用：把 worker 的结果送回 TinyUSB。 */
void ra8_msc_poll(void) {
    const uint32_t _irq = msc_irq_save();
    if (msc_io.state == MSC_IO_DONE) {
        if (msc_io.generation == msc_generation) {
            int32_t result = msc_io.result;
            const bool writing = msc_io.writing;
            if (result > 0 && !writing) {
                memcpy(msc_io.usb_buffer, msc_bounce, (uint32_t) result);
            }
            if (result < 0) {
                tud_msc_set_sense(msc_io.lun, SCSI_SENSE_MEDIUM_ERROR, writing ? 0x0c : 0x11, 0x00);
                result = TUD_MSC_RET_ERROR;
            }
            msc_io.state = MSC_IO_IDLE;
            (void) tud_msc_async_io_done(result, false);
        } else {
            /* 复位已作废 */
            msc_io.state = MSC_IO_IDLE;
        }
    }
    msc_irq_restore(_irq);
}

/* ---- 回调 ---- */

void tud_msc_inquiry_cb(uint8_t lun, uint8_t vendor_id[8], uint8_t product_id[16], uint8_t product_rev[4]) {
    (void) lun;
    const char *vendor = MICROPY_HW_USB_MSC_INQUIRY_VENDOR_STRING;
    const char *product = MICROPY_HW_USB_MSC_INQUIRY_PRODUCT_STRING;
    const char *revision = MICROPY_HW_USB_MSC_INQUIRY_REVISION_STRING;

    memcpy(vendor_id, vendor, MIN(strlen(vendor), 8));
    memcpy(product_id, product, MIN(strlen(product), 16));
    memcpy(product_rev, revision, MIN(strlen(revision), 4));
}

/* 以下三个只在 USB 任务里被调用，只读缓存值/标志，不做介质访问。 */

bool tud_msc_test_unit_ready_cb(uint8_t lun) {
    if (ejected || !sdcard_inserted) {
        tud_msc_set_sense(lun, SCSI_SENSE_NOT_READY, 0x3a, 0x00);
        return false;
    }
    return true;
}

void tud_msc_capacity_cb(uint8_t lun, uint32_t *block_count, uint16_t *block_size) {
    (void) lun;
    *block_size = (uint16_t) SDCard_GetSectorSize();
    *block_count = SDCard_GetSectorCount();
}

bool tud_msc_start_stop_cb(uint8_t lun, uint8_t power_condition, bool start, bool load_eject) {
    (void) lun;
    (void) power_condition;
    if (load_eject) {
        ejected = !start;
    }
    return true;
}

static int32_t msc_submit(uint8_t lun, uint32_t lba, uint32_t offset, void *buffer, uint32_t size, bool writing) {
    const uint32_t sector_size = SDCard_GetSectorSize();

    if (offset != 0 || size == 0 || (size % sector_size) != 0 || size > sizeof(msc_bounce)) {
        tud_msc_set_sense(lun, SCSI_SENSE_ILLEGAL_REQUEST, 0x21, 0x00);
        return -1;
    }

    const uint32_t _irq = msc_irq_save();
    if (!msc_worker_started || msc_io.state != MSC_IO_IDLE) {
        /* 上一次介质操作还没被收走。不忙等，直接让本次 CBW 失败。 */
        msc_irq_restore(_irq);
        tud_msc_set_sense(lun, SCSI_SENSE_NOT_READY, 0x04, 0x01);
        return -1;
    }
    msc_io.generation = msc_generation;
    msc_io.lba = lba;
    msc_io.offset = offset;
    msc_io.size = size;
    msc_io.lun = lun;
    msc_io.writing = writing;
    msc_io.usb_buffer = buffer;
    if (writing) {
        memcpy(msc_bounce, buffer, size);
    }
    msc_io.state = MSC_IO_QUEUED;
    msc_irq_restore(_irq);

    xSemaphoreGive(msc_io_ready);
    return TUD_MSC_RET_ASYNC;
}

int32_t tud_msc_read10_cb(uint8_t lun, uint32_t lba, uint32_t offset, void *buffer, uint32_t bufsize) {
    return msc_submit(lun, lba, offset, buffer, bufsize, false);
}

int32_t tud_msc_write10_cb(uint8_t lun, uint32_t lba, uint32_t offset, uint8_t *buffer, uint32_t bufsize) {
    return msc_submit(lun, lba, offset, buffer, bufsize, true);
}

int32_t tud_msc_scsi_cb(uint8_t lun, uint8_t const scsi_cmd[16], void *buffer, uint16_t bufsize) {
    (void) buffer;
    (void) bufsize;
    switch (scsi_cmd[0]) {
        case SCSI_CMD_PREVENT_ALLOW_MEDIUM_REMOVAL:
            return 0;
        default:
            tud_msc_set_sense(lun, SCSI_SENSE_ILLEGAL_REQUEST, 0x20, 0x00);
            return -1;
    }
}

#endif // CFG_TUD_MSC

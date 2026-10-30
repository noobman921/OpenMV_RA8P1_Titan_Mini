/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (C) 2025 OpenMV, LLC.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software and to permit persons to whom the Software is
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
 * RA8P1 OpenMV + MicroPython main entry (FreeRTOS task).
 */
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#include "py/builtin.h"
#include "py/compile.h"
#include "py/runtime.h"
#include "py/repl.h"
#include "py/gc.h"
#include "py/mperrno.h"
#include "py/stackctrl.h"
#include "shared/runtime/pyexec.h"
#include "shared/runtime/softtimer.h"
#include "shared/readline/readline.h"
#include "gccollect.h"
#include "extmod/vfs.h"
#include "extmod/vfs_fat.h"
#include "FreeRTOS.h"
#include "task.h"

#include "omv_boardconfig.h"
#include "mpconfigboard.h"   /* MICROPY_HW_USB_MSC —— MSC worker 的开关 */
#include "umalloc.h"         /* uma_init / uma_pool_add / UMA_* 标志 */
#include "omv_gpio.h"
#include "omv_i2c.h"
#include "omv_csi.h"
#include "omv_protocol.h"
#include "mp_utils.h"
#include "dma_alloc.h"
#include "file_utils.h"

#include "py_image.h"
#include "py_fir.h"
#include "py_tv.h"
#include "py_imu.h"

#include "bsp_api.h"
#include "tusb.h"


#include "hal_data.h"
#include "drivers/fsp_sdcard.h"

int errno;

void NORETURN __fatal_error(const char *msg) {
    for (volatile uint32_t delay = 0;; delay++) {
        if (delay > 10000000) {
            __WFI();
        }
    }
}

void nlr_jump_fail(void *val) {
    printf("FATAL: uncaught exception %p\n", val);
    __fatal_error("");
}

#ifndef NDEBUG
void __attribute__((weak)) __assert_func(const char *file, int line, const char *func, const char *expr) {
    (void) func;
    printf("Assertion '%s' failed, at file %s:%d\n", expr, file, line);
    __fatal_error("");
}
#endif

#ifdef STACK_PROTECTOR
uint32_t __stack_chk_guard = 0xDEADBEEF;

void NORETURN __stack_chk_fail(void) {  
    __fatal_error("stack check failed");
}
#endif

static fs_user_mount_t sdcard_flash_fs;

// FatFS 的 f_mkfs 生成的 MBR 在偏移 0x1B8 处的磁盘签名（Disk Signature）留空为 0，
// 卷序列号又来自 get_fattime()（本工程返回 0），两者 Windows 都当作"卷身份无效"。
// 这里在挂载后补齐；已非 0 则不改（幂等），且只写卷外的 LBA0 与 VBR 首扇区。
static void sdcard_patch_volume_identity(void) {
    static uint8_t blk[512] __attribute__((aligned(32)));

    if (SDCard_ReadBlocks(0, blk, 1) != 0 || blk[510] != 0x55 || blk[511] != 0xAA) {
        return;
    }

    // MBR 磁盘签名 @ 0x1B8
    uint32_t cur_sig = blk[0x1B8] | (blk[0x1B9] << 8) |
                       ((uint32_t)blk[0x1BA] << 16) | ((uint32_t)blk[0x1BB] << 24);
    if (cur_sig == 0) {
        uint32_t sig = 0x52413850u ^ SDCard_GetSectorCount();
        if (sig == 0) {
            sig = 0x52413850u;
        }
        blk[0x1B8] = sig & 0xFF;
        blk[0x1B9] = (sig >> 8) & 0xFF;
        blk[0x1BA] = (sig >> 16) & 0xFF;
        blk[0x1BB] = (sig >> 24) & 0xFF;
        SDCard_WriteBlocks(0, blk, 1);
    }

    uint32_t part = blk[0x1BE + 8] | (blk[0x1BE + 9] << 8) |
                    ((uint32_t)blk[0x1BE + 10] << 16) | ((uint32_t)blk[0x1BE + 11] << 24);
    if (part == 0 || SDCard_ReadBlocks(part, blk, 1) != 0) {
        return;
    }
    if (blk[510] != 0x55 || blk[511] != 0xAA) {
        return;
    }

    // 卷序列号 @ 67 (FAT32 BS_VolID32)
    uint32_t cur_vsn = blk[67] | (blk[68] << 8) | ((uint32_t)blk[69] << 16) | ((uint32_t)blk[70] << 24);
    if (cur_vsn == 0) {
        uint32_t vsn = 0x1A2B3C4Du ^ (SDCard_GetSectorCount() >> 3);
        if (vsn == 0) {
            vsn = 0x1A2B3C4Du;
        }
        blk[67] = vsn & 0xFF;
        blk[68] = (vsn >> 8) & 0xFF;
        blk[69] = (vsn >> 16) & 0xFF;
        blk[70] = (vsn >> 24) & 0xFF;

        uint16_t bkboot = blk[50] | (blk[51] << 8);
        SDCard_WriteBlocks(part, blk, 1);
        if (bkboot) {
            static uint8_t bk[512] __attribute__((aligned(32)));
            if (SDCard_ReadBlocks(part + bkboot, bk, 1) == 0) {
                bk[67] = blk[67];
                bk[68] = blk[68];
                bk[69] = blk[69];
                bk[70] = blk[70];
                SDCard_WriteBlocks(part + bkboot, bk, 1);
            }
        }
    }
}



static int sdcard_flash_init_fs(void) {
    fsp_err_t fsp_err = SDCard_Init();
    if (FSP_SUCCESS != fsp_err) {
        return -1;
    }


    fs_user_mount_t *vfs_fat = &sdcard_flash_fs;
    vfs_fat->blockdev.flags = 0;
    sdcard_flash_init_vfs(vfs_fat);

    FRESULT res = f_mount(&vfs_fat->fatfs);

    // 卡上可能是先前留下的异常小分区（Windows 只认到几 MB），此时整卡重格式化
    bool force_format = false;
    if (res == FR_OK) {
        FATFS *fs = &vfs_fat->fatfs;
        // n_fatent / csize 在 f_mount 时已从 BPB 读出，无需扫描整个 FAT
        uint32_t total_mb = (uint32_t)(((uint64_t)(fs->n_fatent - 2) * fs->csize) / 2048);
        printf("SD: volume size = %lu MB\n", (unsigned long)total_mb);
        if (total_mb < 1000) {
            printf("SD: volume too small, reformatting whole card\n");
            force_format = true;
        }
    }

    if (res == FR_NO_FILESYSTEM || force_format) {
        static uint8_t working_buf[FF_MAX_SS];
        printf("SD: formatting card as FAT32 (MBR + partition at LBA 63)\n");
        res = f_mkfs(&vfs_fat->fatfs, FM_FAT32, 0, working_buf, sizeof(working_buf));
        if (res == FR_OK) {
            res = f_mount(&vfs_fat->fatfs);
            printf("SD: format done\n");
        } else {
            printf("SD: format failed (%d)\n", res);
        }
    }
    if (res != FR_OK) return -1;

    sdcard_patch_volume_identity();

    mp_vfs_mount_t *vfs = m_new_obj_maybe(mp_vfs_mount_t);
    if (vfs == NULL) {
        return -1;
    }
    vfs->str = "/sdcard";
    vfs->len = 7;
    vfs->obj = MP_OBJ_FROM_PTR(vfs_fat);
    vfs->next = MP_STATE_VM(vfs_mount_table);   // 保留已有挂载（/rom 等），插到表头
    MP_STATE_VM(vfs_mount_table) = vfs;
    return 0;
}

// extern void mipi_csi_test(void);

// SDRAM 布局（完整表见 script/fsp.ld 顶部注释）：
//   0x68000000 .. 0x681C2000  VIN 三帧采集缓冲，MPU 标记 nocache，不可占用
//   0x681C2000 .. 0x68600000  4.24MB  → GC
//   0x68600000 .. 0x68680000  IDE 流缓冲 SB（framebuffer.c 的 _sb_memory_*），不可占用
//   0x68680000 .. 0x68800000  2MB     → GC
//   0x68800000 .. 0x68A00000  OMV_CODE，不可占用
//   0x68A00000 .. 0x69200000  UMA 主池，不可占用
//   0x69200000 .. 0x6A000000  14MB    → GC
//
// 前两段原先空置的原因：
//   · 0x68400000..0x68600000 原叫"FB 主池"。uma 取代 fb_alloc 后，主 framebuffer
//     改成 dynamic=true 从 uma 分配（framebuffer.c:169-178），这块就没人用了。
//   · 0x68680000..0x68800000 是 fb_alloc overlay 的尾部，同样早已空置。
// 三处都由 map 核实过：除调试符号外无任何输出段落在其中。
//
// 分成三段而不是一整块，是因为中间夹着 SB 流缓冲、OMV_CODE 和 UMA 池。
// MICROPY_GC_SPLIT_HEAP 支持任意多个区，分配时按顺序扫，够大就能用。
typedef struct {
    uintptr_t start;
    uintptr_t end;
} omv_gc_region_t;

static const omv_gc_region_t omv_gc_sdram_regions[] = {
    { 0x681C2000u, 0x68600000u },   // 4.24 MB，VIN 缓冲之后
    { 0x68680000u, 0x68800000u },   // 2 MB，SB 之后、OMV_CODE 之前
    { 0x69200000u, 0x6A000000u },   // 14 MB，UMA 池之后
};

// 校验：先写唯一图案再读回。SDRAM 若实际容量小于映射范围，高地址会别名到
// 低地址，写入互相覆盖，读回必然对不上 —— 这也正是要拦的情况。
// 按 64 字节步长采样而不是逐字：别名周期是 2 的幂且 >= 1MB，64 字节足以发现，
// 同时把三段的校验总量从 20MB 降到 320KB，开机不额外多花半秒。
#define OMV_GC_CHECK_STRIDE_WORDS  (16u)

// GC 堆是纯 CPU 访问（无 DMA），可缓存反而是好事，无需 cache 维护。
// 校验失败的那一段跳过不注册，其余照常并入 —— 避免一段坏内存拖垮整个堆。
static void omv_gc_add_sdram(void) {
    for (size_t r = 0; r < sizeof(omv_gc_sdram_regions) / sizeof(omv_gc_sdram_regions[0]); r++) {
        const uintptr_t start = omv_gc_sdram_regions[r].start;
        const uintptr_t end = omv_gc_sdram_regions[r].end;
        volatile uint32_t *p = (volatile uint32_t *) start;
        const uint32_t words = (uint32_t) ((end - start) / sizeof(uint32_t));
        bool ok = true;

        for (uint32_t i = 0; i < words; i += OMV_GC_CHECK_STRIDE_WORDS) {
            p[i] = i ^ 0xA5A5A5A5u;
        }
        for (uint32_t i = 0; i < words; i += OMV_GC_CHECK_STRIDE_WORDS) {
            if (p[i] != (i ^ 0xA5A5A5A5u)) {
                printf("GC: SDRAM check FAILED @0x%08lx got 0x%08lx, region skipped\n",
                       (unsigned long) (start + i * sizeof(uint32_t)),
                       (unsigned long) p[i]);
                ok = false;
                break;
            }
        }
        if (!ok) {
            continue;
        }

        gc_add((void *) start, (void *) end);
        printf("GC: +%lu KB SDRAM heap @0x%08lx\n",
               (unsigned long) ((end - start) / 1024), (unsigned long) start);
    }
}


extern int ra8_npu_init(void);

/* ============================================================================
 * 服务线程：USB 泵 + 软定时器
 * ============================================================================
 * 一个任务干两件事，因为它们都必须在【高于 MPY Thread(3)】的优先级上跑。
 *
 * —— 为什么必须高于 3（这是本工程一处隐蔽的饿死结构）——
 * REPL 在 MPY Thread(优先级 3) 上执行 mp_hal_stdin_rx_chr()，那是一个
 * 【永不阻塞】的忙循环（ports/renesas-ra/uart_core.c:41-60）：
 *     for(;;){ ...; MICROPY_EVENT_POLL_HOOK; }   // = mp_handle_pending(); __WFI();
 * mp_handle_pending() 与 __WFI() 都【不会让出给更低优先级的任务】—— __WFI 只让
 * CPU 睡到下一个中断，醒来后调度器仍挑最高优先级的就绪任务，也就是它自己。
 * 于是任何优先级 < 3 的任务，在 REPL 等待输入期间【永远轮不到】。
 *
 * 实测证据（2026-09-22，tools/usb_probe/probe_idle_state.jlink，37 帧 / 50 秒）：
 *     xTickCount 正常递增（0x1EE0 → 0xE034），uwTick 却恒为 0x4C2 不动。
 * uwTick 的唯一写入者就是本任务 → 它从未被调度过。
 *
 * 后果：soft_timer 全链路死掉（时间基不走 + soft_timer_handler() 无人驱动），
 * 建在其上的协议层 poll_ms 周期泵、各通道 tick 从未执行过。此前把本任务放在
 * 优先级 2（低于 MPY Thread）在结构上就不可能生效 —— 这正是本项目反复出现
 * 「没人来推进协议」类症状的根源。
 *
 * —— 为什么不能照抄 Titan 的 `for(;;) tud_task_ext(1, false);` ——
 * 本工程 CFG_TUSB_OS = OPT_OS_NONE，osal/osal_none.h:186-193 的
 * osal_queue_receive 把 msec 直接丢弃（原文 *"not used, always behave as
 * msec = 0"*），tud_task_ext 根本不会睡，超时参数形同虚设。Titan 能那么写是
 * 因为 RT-Thread 的 OSAL 真会睡。这里必须自己 vTaskDelay() 让出 CPU。
 *
 * 并发安全依赖 mphalport.h 里补的 MICROPY_BEGIN/END_ATOMIC_SECTION。
 * ==========================================================================*/
static void ra8_svc_task(void *pvParameters) {
    FSP_PARAMETER_NOT_USED(pvParameters);
    for (;;) {
        /* 把事件队列**排空**再让出，而不是一次调用就睡。
         * tud_task_ext() 单次最多处理 CFG_TUD_TASK_EVENTS_PER_RUN 个事件
         * （usbd.c:680-686）；若每 1ms 只调一次，突发时事件会在 16 深的队列里
         * 堆积，满了就被 queue_event 的 TU_ASSERT 静默丢弃 —— 丢掉一个 OUT 端的
         * XFER_COMPLETE，端点就不再武装，主机的后续字节永远送不进来。
         * 对应 Titan：它的 usb_thread 是紧循环 `tud_task_ext(1,false)` 连续泵。 */
        while (tud_task_event_ready()) {
            tud_task_ext(0, false);
        }
#if MICROPY_HW_USB_MSC
        /* U 盘的介质 I/O 由 msc_io worker 做，这里只把结果送回 TinyUSB。
         * 必须在 tud_task_ext() 之后调用（对应 Titan ra8_usb.c 的 usb_thread）。 */
        ra8_msc_poll();
#endif
        /* 时间基已改为直接取 FreeRTOS tick（见 systick.c 的 soft_timer_get_ms），
         * 本任务不再需要维护 uwTick —— 那个变量曾经一被饿死就让所有软定时器停摆。 */
        soft_timer_handler();

        vTaskDelay(1);
    }
}

/* 给低优先级的 msc_io worker 让出 CPU。
 * 由 mpconfigport.h 的 MICROPY_EVENT_POLL_HOOK 调用 —— REPL 的等待循环是
 * 不阻塞的，不让出的话优先级 2 的 worker 永远没机会跑，U 盘命令永远完不成。 */
void ra8_vm_yield_if_needed(void) {
#if MICROPY_HW_USB_MSC
    extern bool ra8_msc_needs_service(void);
    if (ra8_msc_needs_service()) {
        vTaskDelay(1);
    }
#endif
}

/* ----------------------------------------------------------------------------
 * 10ms 的 VM 轮询节拍
 * ----------------------------------------------------------------------------
 * 对应 Titan_Mini_Openmv/platform/micropython/mphal.c:70-80 的 vm_poll_timer。
 *
 * 为什么需要它：调度节点（mp_sched_schedule_node）只有在 **VM 检查待办**时
 * 才会被运行。所以「让出」必须有东西**周期性**地挂上节点，否则：
 *   - Python 忙循环：靠协议节点（50ms）也能走到，但 50ms 对 U 盘太粗；
 *   - 纯 C 代码执行期间（传感器初始化等）：完全没人挂节点。
 * 10ms 挂一次，配合上面 ra8_vm_yield_if_needed()，把 worker 的响应延迟
 * 从 50ms 压到 10ms —— 这正是 U 盘挂载偏慢的直接原因。
 *
 * 节拍由 svc 任务里的 soft_timer_handler() 驱动（与软定时器同一套基础设施），
 * 所以本定时器在有无 IDE 会话时都会准时触发。 */
#define RA8_VM_POLL_MS  (10)
static soft_timer_entry_t vm_poll_timer;
static mp_sched_node_t vm_poll_node;

/* 协议轮询的重入标志 + 异常清场回调。
 *
 * 为什么必须有（对齐 Titan_Mini_Openmv/platform/micropython/mphal.c:96-113 的
 * protocol_polling / poll_cleanup）：
 *
 *   omv_protocol_send_packet() 等 ACK 的循环体里会调
 *   OMV_PROTOCOL_EVENT_POLL()（= mp_event_handle_nowait()）→
 *   mp_handle_pending(true) → mp_sched_run_pending() → 又跑到本函数 →
 *   omv_protocol_poll() **递归重入**。
 *
 * 上游的 omv_protocol_poll() 只在开头挡了 ISR 重入（__get_IPSR() != 0），
 * 挡不住线程内的递归。重入时内层会再次 siphon 数据进 ctx.buffer，而外层
 * 可能正拿着指向该缓冲的 packet 指针；也可能在 send_packet 进行中撞上
 * 内层触发的 omv_protocol_reset()（缓冲区被清、state/sequence 被重置）。
 *
 * 用 nlr_jump_callback 而不是简单的 flag=0 收尾：协议路径会因 IDE 的 STOP
 * 或内存不足 longjmp 出去，flag 卡在 true 就再也不轮询了（永久断连）。 */
static volatile bool ra8_protocol_polling;

static void ra8_protocol_poll_cleanup(void *arg) {
    (void) arg;
    ra8_protocol_polling = false;
}

static void ra8_vm_poll_task(mp_sched_node_t *node) {
    (void) node;

    /* 重入保护：见上面 ra8_protocol_polling 的说明。 */
    if (ra8_protocol_polling) {
        return;
    }
    ra8_hal_poll();
    ra8_vm_yield_if_needed();
}

/* 协议轮询主体：会话同步 + 协议状态机。由 ra8_vm_poll_task 每 10ms 调用。 */
void ra8_hal_poll(void) {
    if (ra8_protocol_polling) {
        return;
    }
    nlr_jump_callback_node_t cleanup;
    nlr_push_jump_callback(&cleanup, ra8_protocol_poll_cleanup);
    ra8_protocol_polling = true;

    /* 会话变更必须最先应用，且必须在任何东西读协议状态之前 ——
     * 对应 Titan_Mini_Openmv/platform/micropython/mphal.c:96-104 的
     *   "Session changes belong to the VM, before timers/protocol can read stale packets."
     *
     * 顺序不可颠倒：通道的 is_active() 带 `applied_epoch == session_epoch` 条件，
     * epoch 一变它立刻返回 false。只有 ra8_usb_session_poll() 把
     * omv_protocol_reset() 跑完并把 applied_epoch 追上去，通道才会重新变活。 */
#if OMV_USB_CHANNEL_RA8
    extern void ra8_usb_session_poll(void);
    ra8_usb_session_poll();
#endif

    /* 上游 1.7.3 把协议任务入口从 omv_protocol_task() 改名为 omv_protocol_poll()。 */
    extern int omv_protocol_poll(void);
    extern bool omv_protocol_is_active(void);
    if (omv_protocol_is_active()) {
        omv_protocol_poll();
    }

    nlr_pop_jump_callback(true);
    ra8_protocol_polling = false;
}

static void ra8_vm_poll_tick(soft_timer_entry_t *self) {
    (void) self;
    mp_sched_schedule_node(&vm_poll_node, ra8_vm_poll_task);
}

/* DTCM 快速池 —— 对应 Titan_Mini_Openmv/platform/micropython/runtime.c:36 的
 * `static uint8_t fast_pool[80*1024] __attribute__((section(".bss.dtcm"), ...))`。
 *
 * 落进链接脚本的 `__dtcm_noinit$$` 段（script/fsp_gen_omv.ld:670，`*(.dtcm_noinit)`，
 * NOLOAD）→ 不占镜像空间，地址在 DTCM（0x2000_0000 起）。
 * DTCM 总 128 KB，`__dtcm_zero$$` 当前占 0x4448（约 17 KB），本池 64 KB
 * → 合计约 97 KB，余量充足（加池后若链接报 "will not fit" 就说明超了）。
 *
 * 为什么必须单独有这样一块：上游 imlib 用 UMA_FAST / UMA_DTCM 请求小块热数据
 * （滤波系数表、行缓冲等，见 filter.c / bmp.c / ppm.c）。这些是内层循环里的
 * 高频访问对象，放 DTCM 才有意义；只靠 SDRAM 池兜底虽能跑，但会明显变慢。 */
static uint8_t uma_fast_pool[64 * 1024] __attribute__((section(".dtcm_noinit"), aligned(32)));

/* OpenMV + MicroPython main entry — called from FreeRTOS task */
void omv_main(void *pvParameters) {
    FSP_PARAMETER_NOT_USED(pvParameters);
    bool first_soft_reset = true;

    /* One-time hardware init */
    board_init();
    mp_uart_repl_init();

    /* 服务线程：只建一次（在 soft_reset 标签之前）。优先级 4 > MPY Thread 的 3。 */
    xTaskCreate(ra8_svc_task, "svc", 1024, NULL, 4, NULL);

#if MICROPY_HW_USB_MSC
    /* U 盘介质 worker：优先级 2，低于 VM 与 svc，必须在调度器里建。 */
    extern void ra8_msc_init(void);
    ra8_msc_init();
#endif

#if MICROPY_HW_ENABLE_USBDEV
    /* Titan Mini V1.0 原理图：P413 驱动 U3 TS3USB221（数据选通）和 U2 CH443K（VBUS）。
     * LOW 选 USBFS，HIGH 选 USBHS。必须与 mpconfigport.h 里选的 rhport 一致，
     * 且在 tusb_init() 让控制器拉起前就锁存好 —— 两个控制器同时上拉会打架。
     * 放在运行时而不是 configuration.xml，是为了跟着 rhport 自动走，
     * 免得每次切换 FS/HS 都要回去改引脚配置。
     * （对应 Titan_Mini_Openmv/platform/drivers/ra8_usb_board.c） */
#if defined(CFG_TUSB_RHPORT1_MODE)
    R_IOPORT_PinCfg(&g_ioport_ctrl, BSP_IO_PORT_04_PIN_13,
                    IOPORT_CFG_PORT_DIRECTION_OUTPUT | IOPORT_CFG_PORT_OUTPUT_HIGH);
#else
    R_IOPORT_PinCfg(&g_ioport_ctrl, BSP_IO_PORT_04_PIN_13,
                    IOPORT_CFG_PORT_DIRECTION_OUTPUT | IOPORT_CFG_PORT_OUTPUT_LOW);
#endif

    /* USB 中断优先级必须显式设置，否则停在复位默认值 0（= 最高优先级）。
     * 实测（J-Link 读 NVIC->IPR[16]）改动前就是 0x00。0 高于 FreeRTOS 的可屏蔽门限
     * configMAX_SYSCALL_INTERRUPT_PRIORITY = 1 << (8 - __NVIC_PRIO_BITS)，也就是
     * taskENTER_CRITICAL() / portSET_INTERRUPT_MASK_FROM_ISR() 用的 BASEPRI 挡不住它
     * —— 内核临界区会被 USB ISR 穿透。置 10 后落回可屏蔽区间。
     * 必须在 tusb_init() 之前：rusb2 的 dcd_init() 会 NVIC_EnableIRQ，
     * 优先级要先到位。（对应 Titan_Mini_Openmv/platform/openmv/ra8_usb.c:158-162） */
#if defined(CFG_TUSB_RHPORT1_MODE)
    R_BSP_IrqCfg(USBHS_USB_INT_RESUME_IRQn, 10, NULL);
#else
    R_BSP_IrqCfg(USBFS_INT_IRQn, 10, NULL);
    R_BSP_IrqCfg(USBFS_RESUME_IRQn, 10, NULL);
#endif

    /* SD 卡的**硬件**初始化必须早于 tusb_init()。
     *
     * 为什么：MSC（U 盘）在枚举时就会回答 GET CAPACITY / TEST UNIT READY，
     * 而这两个回答直接取决于 sdcard_sector_count / sdcard_inserted。
     * 如果 USB 先起来、SD 后初始化，Windows 会在枚举时看到一个
     * **0 块、无介质**的可移动盘 → 盘符出现但虚化、打不开；
     * 而且它会把容量缓存下来，不重新枚举就不重读 —— 这正是实测到的
     * 「只有第一次断连后才变成正常 U 盘」。
     * 更糟的是：挂不上盘会让 Windows 超时后**复位端口**，总线复位随即
     * 把 CDC 会话一起打掉（探针实测会话死于 clrSrc=1 的总线复位）。
     *
     * 只做硬件初始化（R_SDHI_Open + MediaInit，无 GC/FatFS 依赖）；
     * 文件系统挂载仍在 soft_reset 之后（需要 gc_init 出来的堆）。
     * SDCard_Init() 已做幂等，后面 sdcard_flash_init_fs() 里再调是 no-op。 */
#if MICROPY_HW_USB_MSC
    SDCard_Init();
#endif

    if (!tusb_inited()) {
        tusb_init();

        /* TX 完成信号量必须在通道被使用前创建。
         * （Titan 在 ra8_usb_start() 里做；我们的 tusb 初始化在这里。） */
#if OMV_USB_CHANNEL_RA8
        extern int ra8_usb_channel_init(void);
        ra8_usb_channel_init();
#endif

            }

#endif

    /* Ethos-U55 NPU: 一次性初始化，之后 TFLM 遇到 Vela 编译过的模型会自动走它。
     * 只在启动时做一次，soft reset 不需要重来（同 tusb_init）。 */
    ra8_npu_init();
    
// mipi test
//    mipi_csi_test();
//    while(1){
//
//    }
soft_reset:

    /* Initialize stack control for this FreeRTOS task */
    mp_cstack_init_with_sp_here(0x8000 - 2048);

    /* GC init */
    gc_init(MICROPY_HEAP_START, MICROPY_HEAP_END);
    omv_gc_add_sdram();

    /* OpenMV low-level subsystems */
    imlib_init();
    readline_init0();
    framebuffer_init0();

#if MICROPY_PY_CSI
    omv_csi_init0();
#endif
#if OMV_DMA_ALLOC
    dma_alloc_init0();
#endif
//#ifdef IMLIB_ENABLE_IMAGE_FILE_IO
//    file_buffer_init0();
//#endif

    /* MicroPython init */
    mp_init();

    /* ---- UMA（通用内存分配器）初始化 ------------------------------------
     * OpenMV 1.7.3 起，imlib 与模块层用 uma（TLSF 多池）取代了 fb_alloc。
     * 本工程已**完成全量迁移**：imlib（含 apriltag）、modules、drivers 里的
     * fb_alloc 调用点全部改为 uma，fb_alloc.c/h 与 umm_malloc.c/h 已删除。
     *
     * 为什么是**两个**池：uma_pool_find（umalloc.c:114-145）的匹配规则要求
     * 请求的标志位必须能在某个池上匹配到，否则返回 NULL 并 uma_fail() 抛
     * MemoryError。上游 imlib 会请求这些组合（已核实）：
     *   UMA_CACHE                  （jpege/draw/qrcode/dmtx/zbar）
     *   UMA_DTCM                   （bmp/filter/ppm/apriltag_config）
     *   UMA_FAST | UMA_CACHE       （draw.c:3188）
     *   UMA_PERSIST | UMA_MAYBE    （framebuffer.c:177）
     * 注意 UMA_CACHE / PERSIST / MAYBE 是**分配**标志（bit8+），会被
     * uma_pool_find 用 UMA_MEM_ATTR_MASK 掩掉；真正参与匹配的只有
     * UMA_FAST / UMA_DTCM 这些**内存属性**位。
     * 所以只注册一个 flags=0 的池时，UMA_FAST / UMA_DTCM 的请求会直接失败。
     *
     * 池布局对照 Titan_Mini_Openmv/platform/micropython/runtime.c:36-40,104-106：
     *   image_heap  8MB SDRAM  flags=0                    ← 我们：UMA_DEFAULT
     *   fast_pool  80KB DTCM   flags=UMA_FAST|UMA_DTCM   ← 我们：64KB，同标志
     * 本工程给 SDRAM 池额外加了 UMA_DEFAULT，让它同时充当任何未匹配请求的
     * 兜底（umalloc.c:140）；Titan 没加，但加了更稳。 */
    {
        extern uint8_t _uma_pool_start[];
        extern uint8_t _uma_pool_end[];
        uma_init();
        uma_pool_add(_uma_pool_start,
                     (size_t) (_uma_pool_end - _uma_pool_start),
                     UMA_DEFAULT);
        uma_pool_add(uma_fast_pool, sizeof(uma_fast_pool),
                     UMA_FAST | UMA_DTCM);
    }

    /* Init OpenMV protocol */
    omv_protocol_init_default();

    /* 10ms VM 轮询节拍。放在这里（soft_reset 之后）而不是标签之前：
     * 软复位路径末尾会调 soft_timer_deinit() 清空定时器堆，放前面会在
     * 第一次软复位后失效。与协议自己的 50ms 定时器同样的处理方式。 */
    soft_timer_remove(&vm_poll_timer);          // 未挂链时是 no-op
    soft_timer_static_init(&vm_poll_timer, SOFT_TIMER_MODE_PERIODIC,
                           RA8_VM_POLL_MS, ra8_vm_poll_tick);
    soft_timer_insert(&vm_poll_timer, RA8_VM_POLL_MS);

    /* Init camera if first boot */
#if MICROPY_PY_CSI
    if (first_soft_reset) {
        int ret = omv_csi_init();
        if (ret != 0 && ret != OMV_CSI_ERROR_ISC_UNDETECTED) {
            __fatal_error("Failed to init the CSI");
        }
    }
#endif

    if (pyexec_mode_kind == PYEXEC_MODE_RAW_REPL) {
        mp_hal_set_interrupt_char(CHAR_CTRL_C);
    }

    int sdcard_fs_ret = sdcard_flash_init_fs();


    while (!omv_protocol_exec_script()) {
        nlr_buf_t nlr;

        if (nlr_push(&nlr) == 0) {
            mp_hal_set_interrupt_char(CHAR_CTRL_C);

            if (pyexec_mode_kind == PYEXEC_MODE_RAW_REPL) {
                if (pyexec_raw_repl() != 0) {
                    break;
                }
            } else {
                if (pyexec_friendly_repl() != 0) {
                    break;
                }
            }
            nlr_pop();
        }
    }

    // soft reset
    mp_hal_set_interrupt_char(-1);
    mp_printf(MP_PYTHON_PRINTER, "MPY: soft reboot\n");
#if MICROPY_PY_CSI
    omv_csi_abort_all();
#endif
    imlib_deinit();
    soft_timer_deinit();
#if MICROPY_HW_ENABLE_USBDEV && MICROPY_HW_ENABLE_USB_RUNTIME_DEVICE
    mp_usbd_deinit();
#endif
    gc_sweep_all();
    mp_deinit();
    first_soft_reset = false;
    goto soft_reset;
}

mp_lexer_t *mp_lexer_new_from_file(qstr filename) {
    mp_raise_OSError(MP_ENOENT);
}

void abort(void) {
    __fatal_error("abort");
}


#if MICROPY_HW_ENABLE_USBDEV
void usbfs_interrupt_handler(void) {
    IRQn_Type irq = R_FSP_CurrentIrqGet();
    R_BSP_IrqStatusClear(irq);

    #if CFG_TUSB_RHPORT0_MODE & OPT_MODE_HOST
    tuh_int_handler(0);
    #endif

    #if CFG_TUSB_RHPORT0_MODE & OPT_MODE_DEVICE
    tud_int_handler(0);
    #endif
}

void usbfs_resume_handler(void) {
    IRQn_Type irq = R_FSP_CurrentIrqGet();
    R_BSP_IrqStatusClear(irq);

    #if CFG_TUSB_RHPORT0_MODE & OPT_MODE_HOST
    tuh_int_handler(0);
    #endif

    #if CFG_TUSB_RHPORT0_MODE & OPT_MODE_DEVICE
    tud_int_handler(0);
    #endif
}

void usbfs_d0fifo_handler(void) {
    IRQn_Type irq = R_FSP_CurrentIrqGet();
    R_BSP_IrqStatusClear(irq);

    #if CFG_TUSB_RHPORT0_MODE & OPT_MODE_HOST
    tuh_int_handler(0);
    #endif

    #if CFG_TUSB_RHPORT0_MODE & OPT_MODE_DEVICE
    tud_int_handler(0);
    #endif
}

void usbfs_d1fifo_handler(void) {
    IRQn_Type irq = R_FSP_CurrentIrqGet();
    R_BSP_IrqStatusClear(irq);

    #if CFG_TUSB_RHPORT0_MODE & OPT_MODE_HOST
    tuh_int_handler(0);
    #endif

    #if CFG_TUSB_RHPORT0_MODE & OPT_MODE_DEVICE
    tud_int_handler(0);
    #endif
}

void usbhs_interrupt_handler(void) {
    IRQn_Type irq = R_FSP_CurrentIrqGet();
    R_BSP_IrqStatusClear(irq);

    #if CFG_TUSB_RHPORT1_MODE & OPT_MODE_HOST
    tuh_int_handler(1);
    #endif

    #if CFG_TUSB_RHPORT1_MODE & OPT_MODE_DEVICE
    tud_int_handler(1);
    #endif
}

void usbhs_d0fifo_handler(void) {
    IRQn_Type irq = R_FSP_CurrentIrqGet();
    R_BSP_IrqStatusClear(irq);

    #if CFG_TUSB_RHPORT1_MODE & OPT_MODE_HOST
    tuh_int_handler(1);
    #endif

    #if CFG_TUSB_RHPORT1_MODE & OPT_MODE_DEVICE
    tud_int_handler(1);
    #endif
}

void usbhs_d1fifo_handler(void) {
    IRQn_Type irq = R_FSP_CurrentIrqGet();
    R_BSP_IrqStatusClear(irq);

    #if CFG_TUSB_RHPORT1_MODE & OPT_MODE_HOST
    tuh_int_handler(1);
    #endif

    #if CFG_TUSB_RHPORT1_MODE & OPT_MODE_DEVICE
    tud_int_handler(1);
    #endif
}
#endif

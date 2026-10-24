/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (C) 2022-2024 OpenMV, LLC.
 *
 * RA8P1 USB 协议通道 —— **移植自 Titan_Mini_Openmv/platform/openmv/ra8_usb.c**
 *
 * ⚠️ 本文件是**平台补丁**，不属于 OpenMV 上游。src/OpenMV/protocol/ 保持与
 *    openmv-master 逐字相同，便于将来整体升级上游。
 *
 * 为什么放在 ports/ 而不是改 protocol/：
 *   这是 Titan 的做法 —— 他们让上游的 omv_protocol_channel_tinyusb.c 因为
 *   OMV_USB_STACK_TINYUSB 未定义而编成空文件，再由自己的 ra8_usb.c 提供
 *   omv_usb_channel。我们照抄：上游那个文件留在原处（编成空），本文件提供实现。
 *
 * 为什么不用上游那份：
 *   多次实测表明断连时 VM 线程会被卡住 1 秒以上（L3 vm_poll_cnt 曾整段为 0），
 *   而上游/旧实现的 usb_channel_read() 会在 VM 线程里循环重试最多 1500ms。
 *   Titan 的 usb_read() 是**非阻塞**的（读多少算多少），write 侧用 TX 完成
 *   信号量代替 vTaskDelay 轮询。这里照搬它的整套通道语义。
 *
 * 与 Titan 原文件的差异（均为平台适配，非语义改动）：
 *   · RT-Thread → FreeRTOS：
 *       rt_hw_interrupt_disable/enable  → PRIMASK（本工程既有做法，ISR 里安全）
 *       rt_sem_init/take/release        → FreeRTOS 二值信号量
 *       rt_tick_from_millisecond        → pdMS_TO_TICKS
 *       rt_interrupt_enter/leave        → 去掉（FreeRTOS 端口自行处理）
 *   · 不创建 usb_thread：本工程的 ra8_svc_task（omvmain.c）已经在泵 tud_task_ext。
 *   · 不提供 usb_start()/USB 中断入口：omvmain.c 已有等价实现，重复定义会冲突。
 *   · **不移植串口 REPL**（ra8_usb_serial_* / serial_rx ring）：
 *     本工程的普通 REPL 仍走 SCI + MicroPython 默认 stdio，不需要它。
 *     因此 tud_cdc_rx_cb 保留「协议未激活时转交 MicroPython」的行为。
 */
#if OMV_USB_CHANNEL_RA8
#include "py/stream.h"
#include "py/mphal.h"
#include "py/ringbuf.h"
#include "py/runtime.h"

#include "omv_common.h"
#include "cmsis_gcc.h"
#include "omv_protocol.h"
#include "omv_boardconfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "tusb.h"
#include "device/dcd.h"
#include "device/usbd_pvt.h"
#if MICROPY_HW_USB_MSC
extern void ra8_msc_reset(void);
#endif

#define OMV_PROTOCOL_USB_CHANNEL_TIMEOUT_MS (1500)

/* ---------------------------------------------------------------------------
 * 会话（session）状态机 —— 与 Titan 逐字对齐
 * ------------------------------------------------------------------------- */
static volatile bool line_dtr;
static volatile bool ide_selected;
static volatile uint32_t session_epoch = 1;
static volatile uint32_t applied_epoch;

/* TX 完成信号量：usb_wait_tx 在这里等，tud_cdc_tx_complete_cb 里释放。
 * 取代旧实现的 vTaskDelay(1) 轮询。 */
static SemaphoreHandle_t cdc_tx_ready;
static volatile bool cdc_tx_waiting;

static void session_changed(void) {
    /* Callback/ISR safe: only publish transport state, never touch VM locks. */
    const uint32_t primask = __get_PRIMASK();
    __disable_irq();
    session_epoch++;
    __set_PRIMASK(primask);
}

/* Suspend pauses the bus; it does not close CDC or replace the IDE session. */
static bool transport_session(void) {
    const bool epoch_ok = (applied_epoch == session_epoch);
    return line_dtr && ide_selected && epoch_ok && tud_mounted();
}

static bool transport_ready(void) {
    return transport_session() && !tud_suspended();
}

/* IDE 用这个波特率标记「这是协议会话，不是普通串口」。
 * 认两个值：IDE 先发 MAGIC(921600)，若 OS/驱动拒绝会改用 12 Mbps 重试。 */
static bool ide_baudrate(uint32_t baud) {
    return baud == OMV_PROTOCOL_MAGIC_BAUDRATE || baud == 12000000;
}

static void discard_old_rx(void) {
    if (tusb_inited() && tud_mounted()) {
        tud_cdc_read_flush();
    }
}

/* ---------------------------------------------------------------------------
 * 通道实现
 * ------------------------------------------------------------------------- */
static size_t usb_channel_size(const omv_protocol_channel_t *channel) {
    (void) channel;
    return transport_ready() ? tud_cdc_available() : 0;
}

static bool usb_channel_is_active(const omv_protocol_channel_t *channel) {
    (void) channel;
    return transport_session();
}

static void usb_wait_tx(uint32_t epoch, uint32_t milliseconds) {
    if (cdc_tx_ready == NULL) {
        /* 信号量还没建（ra8_usb_channel_init 未调用）—— 退化成让出一个 tick，
         * 绝不能对 NULL 句柄做 xSemaphoreTake。 */
        vTaskDelay(1);
        return;
    }
    cdc_tx_waiting = true;
    /* Recheck after publishing the waiter: resume/complete may have happened
     * just before this flag. Never sleep through a known-ready transition. */
    if (epoch == session_epoch && transport_session() &&
        (tud_suspended() || !tud_cdc_write_available())) {
        xSemaphoreTake(cdc_tx_ready, pdMS_TO_TICKS(milliseconds ? milliseconds : 1));
    }
    cdc_tx_waiting = false;
}

static int usb_channel_flush(const omv_protocol_channel_t *channel) {
    (void) channel;
    const uint32_t epoch = session_epoch;
    const uint32_t start = mp_hal_ticks_ms();
    while (epoch == session_epoch && transport_session()) {
        uint32_t elapsed = mp_hal_ticks_ms() - start;
        if (elapsed >= OMV_PROTOCOL_USB_CHANNEL_TIMEOUT_MS) { return -1; }
        if (!tud_suspended()) { return (int) tud_cdc_write_flush(); }
        uint32_t remaining = OMV_PROTOCOL_USB_CHANNEL_TIMEOUT_MS - elapsed;
        usb_wait_tx(epoch, remaining < 10 ? remaining : 10);
    }
    return -1;
}

/* ★ 与旧实现的关键差异：**非阻塞**。
 * 旧的版本在这里 while(...) { tud_cdc_read; vTaskDelay(1); } 最多转 1500ms，
 * 而本函数在 VM 线程里被调用 —— 一卡就是整个 Python 执行停摆。
 * protocol 的 siphon 循环本来就是「有多少读多少、没有就退出、下个节拍再来」，
 * 通道层不需要替它等。 */
static int usb_channel_read(const omv_protocol_channel_t *channel, uint32_t offset, size_t size, void *data) {
    (void) channel; (void) offset;
    if (!transport_ready()) { return 0; }
    return tud_cdc_read(data, size);
}

static int usb_channel_write(const omv_protocol_channel_t *channel, uint32_t offset, size_t size, const void *data) {
    (void) channel; (void) offset;
    size_t sent = 0;
    const uint32_t start = mp_hal_ticks_ms();
    const uint32_t epoch = session_epoch;
    while (sent < size && epoch == session_epoch && transport_session()) {
        if (mp_hal_ticks_ms() - start >= OMV_PROTOCOL_USB_CHANNEL_TIMEOUT_MS) {
            break;
        }
        if (!tud_suspended()) {
            size_t n = tud_cdc_write((const uint8_t *) data + sent, size - sent);
            sent += n;
            if (epoch == session_epoch && transport_ready()) { tud_cdc_write_flush(); }
        }
        if (sent == size) { break; }
        uint32_t elapsed = mp_hal_ticks_ms() - start;
        if (elapsed >= OMV_PROTOCOL_USB_CHANNEL_TIMEOUT_MS) { break; }
        uint32_t remaining = OMV_PROTOCOL_USB_CHANNEL_TIMEOUT_MS - elapsed;
        usb_wait_tx(epoch, remaining < 10 ? remaining : 10);
    }
    return (int) sent;
}

/* ---------------------------------------------------------------------------
 * TinyUSB 回调（会话边界）
 * ------------------------------------------------------------------------- */
void tud_cdc_rx_cb(uint8_t itf) {
    extern void __mp_tud_cdc_rx_cb(uint8_t itf);

    if (!line_dtr) {
        /* 已关闭会话的迟到字节绝不能变成 REPL 的 Ctrl-C。 */
        tud_cdc_read_flush();
        return;
    }
    if (!omv_protocol_is_active()) {
        __mp_tud_cdc_rx_cb(itf);
    }
}

void tud_cdc_line_coding_cb(uint8_t itf, cdc_line_coding_t const *coding) {
    (void) itf;
    const bool selected = ide_baudrate(coding->bit_rate);
    if (selected != ide_selected) {
        ide_selected = selected;
        session_changed();
        discard_old_rx();
    }
}

void tud_cdc_line_state_cb(uint8_t instance, bool dtr, bool rts) {
    extern void __mp_tud_cdc_line_state_cb(uint8_t instance, bool dtr, bool rts);

    (void) rts;
    cdc_line_coding_t coding;
    tud_cdc_get_line_coding(&coding);

    const bool selected = ide_baudrate(coding.bit_rate);
    const bool changed = (dtr != line_dtr) || (selected != ide_selected);
    line_dtr = dtr;
    ide_selected = selected;
    if (changed) { session_changed(); }

    if (dtr) {
        // A stall left over from a previous session would make every CHANNEL_READ
        // of the host fail; clear it as soon as the protocol session is re-opened.
        if (selected && usbd_edpt_stalled(TUD_OPT_RHPORT, USBD_CDC_EP_IN)) {
            usbd_edpt_clear_stall(TUD_OPT_RHPORT, USBD_CDC_EP_IN);
        }
    } else {
        /* TX FIFO may still be owned by an IN transfer. Do not clear it here.
         * Its tail can drain on reopen; bus reset performs a complete reset. */
        discard_old_rx();
    }
    if (cdc_tx_waiting && cdc_tx_ready != NULL) { xSemaphoreGive(cdc_tx_ready); }

    __mp_tud_cdc_line_state_cb(instance, dtr, rts);
}

void tud_cdc_tx_complete_cb(uint8_t itf) {
    (void) itf;
    if (cdc_tx_waiting && cdc_tx_ready != NULL) { xSemaphoreGive(cdc_tx_ready); }
}

void tud_mount_cb(void) {
    line_dtr = false;
    session_changed();
}

void tud_umount_cb(void) {
    line_dtr = false;
    session_changed();
    if (cdc_tx_waiting && cdc_tx_ready != NULL) { xSemaphoreGive(cdc_tx_ready); }
}

void tud_resume_cb(void) {
    if (cdc_tx_waiting && cdc_tx_ready != NULL) { xSemaphoreGive(cdc_tx_ready); }
}

void tud_event_hook_cb(uint8_t rhport, uint32_t eventid, bool in_isr) {
    extern void __mp_tud_event_hook_cb(uint8_t rhport, uint32_t eventid, bool in_isr);

    if (rhport != TUD_OPT_RHPORT) { return; }

    if (eventid == DCD_EVENT_BUS_RESET_START || eventid == DCD_EVENT_BUS_RESET_END ||
        eventid == DCD_EVENT_UNPLUGGED) {
        /* 只标脏 + 自增 epoch。本函数在 **USB 中断**里跑，绝不能碰协议状态。 */
        line_dtr = false;
        session_changed();
#if MICROPY_HW_USB_MSC
        /* 作废在途的 U 盘作业：复位会立刻复用 TinyUSB 的端点缓冲发新 CBW。 */
        ra8_msc_reset();
#endif
        if (cdc_tx_waiting && cdc_tx_ready != NULL) { xSemaphoreGive(cdc_tx_ready); }
    }

    if (!omv_protocol_is_active()) {
        __mp_tud_event_hook_cb(rhport, eventid, in_isr);
    }
}

/* 应用挂起的会话变更。必须在 VM 上下文调用（omvmain.c 的 vm_poll 节点）。 */
extern void omv_protocol_reset(void);

void ra8_usb_session_poll(void) {
    const uint32_t epoch = session_epoch;
    if (applied_epoch == epoch) {
        return;
    }
    omv_protocol_reset();
    applied_epoch = epoch;
}

/* ---------------------------------------------------------------------------
 * 通道注册
 * ------------------------------------------------------------------------- */
const omv_protocol_channel_t omv_usb_channel = {
    .priv = NULL,
    .id = OMV_PROTOCOL_CHANNEL_ID_TRANSPORT,
    .name = "usb",
    .flags = OMV_PROTOCOL_CHANNEL_FLAG_PHYSICAL,
    .size = usb_channel_size,
    .read = usb_channel_read,
    .write = usb_channel_write,
    .flush = usb_channel_flush,
    .is_active = usb_channel_is_active
};

/* 在 tusb_init() 之后调用一次，创建 TX 完成信号量。
 * （Titan 在 ra8_usb_start() 里做；我们的 tusb 初始化在 omvmain.c。） */
int ra8_usb_channel_init(void) {
    if (cdc_tx_ready == NULL) {
        cdc_tx_ready = xSemaphoreCreateBinary();
        if (cdc_tx_ready == NULL) { return -1; }
    }
    return 0;
}
#endif // OMV_USB_CHANNEL_RA8

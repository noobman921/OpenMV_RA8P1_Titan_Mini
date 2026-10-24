/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 Blake W. Felt & Angus Gratton
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
 */

#include "py/mphal.h"

#if MICROPY_HW_ENABLE_USBDEV

#include "mp_usbd.h"

#if !MICROPY_HW_ENABLE_USB_RUNTIME_DEVICE

void mp_usbd_task(void) {
#if MICROPY_HW_USB_DEDICATED_TASK
    /* 本工程有一个专用 USB 任务（omvmain.c 的 ra8_svc_task）在持续泵 tud_task_ext()。
     *
     * ⚠️ 这里【必须】是空操作。tud_task_ext() 不可重入，且本工程
     * OSAL_MUTEX_REQUIRED = 0（tusb_option.h），tinyusb 内部没有任何互斥。
     * 而本函数会被 MP 的 stdio 路径调用：
     *     mp_hal_stdin_rx_chr()（REPL 忙循环，跑在 VM 线程，优先级 3）
     *       → mp_usbd_cdc_poll_interfaces()（mp_usbd_cdc.c:42）
     *         → mp_usbd_task()
     * 于是两个不同优先级的任务会并发进入 tud_task_ext() —— 专用任务优先级更高，
     * 可以在 VM 处理到一半时抢占并再进一次，两边同时读写 _usbd_dev、事件队列
     * 与端点状态。实测后果：CDC OUT 端点的 XFER_COMPLETE 与「重新武装」互相踩，
     * 端点不再武装 → 主机被永久 NAK → 长脚本的包传一半就停住
     * （实测停在 762/1173 字节，短脚本因为只有一个包所以能过）。
     *
     * 对应 Titan：它的 mp_hal_poll() 完全不碰 USB，泵的唯一所有者就是 usb_thread。 */
    return;
#else
    tud_task_ext(0, false);
#endif
}

void mp_usbd_task_callback(mp_sched_node_t *node) {
    (void)node;
    mp_usbd_task();
}

#endif // !MICROPY_HW_ENABLE_USB_RUNTIME_DEVICE

// Schedule the TinyUSB task on demand, when there is a new USB device event
TU_ATTR_FAST_FUNC void MICROPY_WRAP_TUD_EVENT_HOOK_CB(tud_event_hook_cb)(uint8_t rhport, uint32_t eventid, bool in_isr) {
    mp_usbd_schedule_task();
    mp_hal_wake_main_task_from_isr();
}

TU_ATTR_FAST_FUNC void mp_usbd_schedule_task(void) {
    static mp_sched_node_t usbd_task_node;
    mp_sched_schedule_node(&usbd_task_node, mp_usbd_task_callback);
}

void mp_usbd_hex_str(char *out_str, const uint8_t *bytes, size_t bytes_len) {
    size_t hex_len = bytes_len * 2;
    for (int i = 0; i < hex_len; i += 2) {
        static const char *hexdig = "0123456789abcdef";
        out_str[i] = hexdig[bytes[i / 2] >> 4];
        out_str[i + 1] = hexdig[bytes[i / 2] & 0x0f];
    }
    out_str[hex_len] = 0;
}

#endif // MICROPY_HW_ENABLE_USBDEV

/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (C) 2022-2024 OpenMV, LLC.
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
 * OpenMV Protocol Channels — stdio（RA8P1 平台补丁）
 *
 * ⚠️ 本文件是**平台补丁**，不属于 OpenMV 上游。src/OpenMV/protocol/ 保持与
 *    openmv-master 逐字相同；上游那份 omv_protocol_channel_stdio.c 通过
 *    .cproject 的 excluding 串排除在构建外（Titan 的 SConscript:48 同样排除它）。
 *
 * 与上游的唯一差异：环形缓冲溢出时的处理。
 *   上游：rb->iget = 0; rb->iput = 0;   —— 整体清空（破坏性）
 *   本文件：只丢弃刚好够腾出空间的字节 —— 因为主机可能已经采样过
 *           CHANNEL_SIZE，此刻清空会让它随后的 READ 变短并收到 NAK。
 *   这对应历史上「print 输出超过缓冲就被整体冲掉」的问题。
 */

#include "py/mphal.h"
#include "py/nlr.h"
#include "py/gc.h"
#include "py/ringbuf.h"
#include "py/runtime.h"
#include "shared/runtime/interrupt_char.h"
#include "shared/runtime/pyexec.h"

#include "omv_common.h"
#include "omv_protocol.h"
#include "omv_boardconfig.h"

#ifndef OMV_PROTOCOL_STDIO_BUFFER_SIZE
#define OMV_PROTOCOL_STDIO_BUFFER_SIZE  (1024)
#endif

#ifndef OMV_PROTOCOL_STDIO_FLUSH_MS
#define OMV_PROTOCOL_STDIO_FLUSH_MS     (50)
#endif


typedef struct {
    vstr_t vstrbuf;
    bool script_running;
    bool notified;
    uint32_t last_notify_ms;
    ringbuf_t ringbuf;
    uint8_t rawbuf[OMV_PROTOCOL_STDIO_BUFFER_SIZE];
} stdio_channel_context_t;

static stdio_channel_context_t stdio_channel_ctx;

/* 必须由主机显式 ioctl(STDIN_EXEC) 才执行脚本 —— 对应 Titan
 * platform/openmv/titan_stdio.c:18,97-104,113-115 的 exec_requested。
 *
 * 上游的 stdin_channel_exec() 只要 vstr 非空就执行。而本工程的
 * while (!omv_protocol_exec_script()) 在 REPL 因任何原因退出（Ctrl-C、REPL 自己
 * 返回、上传中断）后都会重新检查一次：此时 vstr 里可能只有**半个脚本**，
 * 会被当成完整脚本执行，跑出乱七八糟的状态（传感器复位、文件半写、协议层被
 * 脚本重新初始化）—— 表现在外部就是通道莫名其妙失联。Titan 的原注释：
 *   "A nonempty vstr is not an EXEC." */
static volatile bool exec_requested;

static int stdin_channel_init(const omv_protocol_channel_t *channel) {
    stdio_channel_context_t *ctx = channel->priv;
    ctx->script_running = false;
    exec_requested = false;
    /* 直接覆盖 buf 指针、不先 vstr_clear 旧缓冲：软复位后 gc_init() 重建了堆，
     * 旧指针已经不属于任何 pool，m_del 会走到 gc_free 里的 assert(area) 直接崩。
     * 旧缓冲不需要手动释放 —— 指针被覆盖后它就不可达了，由 GC 正常回收
     * （本缓冲本身已由 omv_stdio_gc_mark() 登记为根）。 */
    vstr_init(&ctx->vstrbuf, 2048);
    return 0;
}

/* 由 ports/renesas-ra/gccollect.c 的 gc_collect() 调用。
 *
 * ctx 是**静态**结构体，GC 只扫描「栈 + 寄存器 + 显式登记的根」，扫不到静态区，
 * 所以 ctx.vstrbuf.buf（vstr_init 用 m_new 分配，在 GC 堆上）没有任何引用 ——
 * 下一次 gc_collect() 就会把它当垃圾回收。之后 vstr_add_len/vstr_reset 写的就是
 * 已释放内存：轻则脚本上传错乱，重则堆结构被踩坏，表现为 UsageFault
 * （UFSR.UNALIGNED）这类随机崩溃。
 *
 * 对应 Titan platform/micropython/runtime.c:84 的 titan_protocol_gc_collect()，
 * 它正是为这个问题加的（titan_stdio.c 用 -Dvstr_init=... 抓同一个指针）。 */
void omv_stdio_gc_mark(void) {
    if (stdio_channel_ctx.vstrbuf.buf != NULL) {
        void *buffer = stdio_channel_ctx.vstrbuf.buf;
        gc_collect_root(&buffer, 1);
    }
}

static int stdout_channel_init(const omv_protocol_channel_t *channel) {
    stdio_channel_context_t *ctx = channel->priv;
    ctx->last_notify_ms = 0;

    // Initialize ring buffer once to keep output from previous runs.
    if (ctx->ringbuf.buf == NULL) {
        ctx->ringbuf = (ringbuf_t) {
            ctx->rawbuf, sizeof(ctx->rawbuf), 0, 0
        };
    }
    return 0;
}

void stdio_channel_pyexec_hook(bool running) {
    stdio_channel_ctx.script_running = running;
    omv_protocol_send_event(OMV_PROTOCOL_CHANNEL_ID_STDIN, running, false);
}

static bool stdin_channel_poll(const omv_protocol_channel_t *channel) {
    stdio_channel_context_t *ctx = channel->priv;
    return ctx->script_running;
}

static bool stdout_channel_poll(const omv_protocol_channel_t *channel) {
    stdio_channel_context_t *ctx = channel->priv;
    return ringbuf_avail(&ctx->ringbuf);
}

static size_t stdio_channel_size(const omv_protocol_channel_t *channel) {
    stdio_channel_context_t *ctx = channel->priv;
    return ringbuf_avail(&ctx->ringbuf);
}

static int stdio_channel_read(const omv_protocol_channel_t *channel,
                              uint32_t offset, size_t size, void *data) {
    stdio_channel_context_t *ctx = channel->priv;

    size = OMV_MIN(size, ringbuf_avail(&ctx->ringbuf));
    int ret = !ringbuf_get_bytes(&ctx->ringbuf, data, size) ? size : -1;
    ctx->notified = false;
    return ret;
}

static int stdio_channel_write(const omv_protocol_channel_t *channel,
                               uint32_t offset, size_t size, const void *data) {
    stdio_channel_context_t *ctx = channel->priv;

    if (offset == 0) {
        exec_requested = false;     // 新一轮上传开始，之前的 EXEC 作废
        vstr_reset(&ctx->vstrbuf);
    }

    nlr_buf_t nlr;
    if (!gc_is_locked() && nlr_push(&nlr) == 0) {
        char *buf = vstr_add_len(&ctx->vstrbuf, size);
        memcpy(buf, data, size);
        nlr_pop();
        return size;
    }

    return -1;
}

static bool stdin_channel_exec(const omv_protocol_channel_t *channel) {
    stdio_channel_context_t *ctx = channel->priv;

    /* 只有主机显式发过 STDIN_EXEC 才执行。见文件上方 exec_requested 的说明。 */
    if (!exec_requested || !vstr_len(&ctx->vstrbuf)) {
        return false;
    }
    exec_requested = false;

    nlr_buf_t nlr;
    if (nlr_push(&nlr) == 0) {
        // Execute the script.
        pyexec_vstr(&ctx->vstrbuf, true);
        nlr_pop();
    } else {
        mp_obj_print_exception(&mp_plat_print, (mp_obj_t) nlr.ret_val);
    }

    vstr_reset(&ctx->vstrbuf);
    return true;
}

static int stdio_channel_ioctl(const omv_protocol_channel_t *channel, uint32_t cmd, size_t len, void *arg) {
    stdio_channel_context_t *ctx = channel->priv;

    switch (cmd) {
        case OMV_CHANNEL_IOCTL_STDIN_STOP:
            exec_requested = false;
            if (mp_interrupt_char != -1) {
                mp_sched_vm_abort();
                mp_sched_keyboard_interrupt();
            }
            break;
        case OMV_CHANNEL_IOCTL_STDIN_EXEC:
            if (!vstr_len(&ctx->vstrbuf)) {
                return -1;
            }
            /* 主机明确要求执行 —— 允许 stdin_channel_exec() 真正跑。 */
            exec_requested = true;
            if (mp_interrupt_char != -1) {
                mp_sched_vm_abort();
                mp_sched_keyboard_interrupt();
            }
            break;
        case OMV_CHANNEL_IOCTL_STDIN_RESET:
            exec_requested = false;
            vstr_reset(&ctx->vstrbuf);
            break;
        default:
            return -1;
    }

    return 0;
}

// Wrap MicroPython stdio functions to intercept REPL.
extern uintptr_t __real_mp_hal_stdio_poll(uintptr_t poll_flags);
uintptr_t __wrap_mp_hal_stdio_poll(uintptr_t poll_flags) {
    if (!omv_protocol_is_active()) {
        return __real_mp_hal_stdio_poll(poll_flags);
    }
    return 0;
}

extern mp_uint_t __real_mp_hal_stdout_tx_strn(const char *str, mp_uint_t len);
mp_uint_t __wrap_mp_hal_stdout_tx_strn(const char *str, mp_uint_t len) {
    if (!omv_protocol_is_active()) {
        return __real_mp_hal_stdout_tx_strn(str, len);
    }

    const omv_protocol_channel_t *channel = omv_protocol_find_channel(OMV_PROTOCOL_CHANNEL_ID_STDOUT);
    stdio_channel_context_t *ctx = channel->priv;

    ringbuf_t *rb = &ctx->ringbuf;
    size_t free = ringbuf_free(rb);

    if (len <= free) {
        // Enough space for the entire string.
        ringbuf_memcpy_put_internal(rb, (const uint8_t *) str, len);
    } else {
        /* 与上游/Titan 一致：溢出时整体清空，只保留本串的尾部。
         *
         * 为什么不能只丢“刚好够”的字节（本文件以前的写法）：
         * 那样环形缓冲会**永久停在满的状态**，而 stdout_channel_tick() 的条件
         * 是「缓冲非空且距上次通知 >= 50ms」→ 于是每 50ms 无条件发一个 NOTIFY。
         * 事件包在 omv_protocol.c:357-362 会被自动加上 ACK_REQ，而
         * omv_protocol_poll() 在等 ACK 期间只处理 ACK 包（omv_protocol.c:505），
         * 控制命令还能挤进去，stdout/预览这些数据通道就被自己的 NOTIFY 洪水
         * 堵死了 —— 表现为「连接还在、暂停/运行正常，但终端和预览全停」。
         * 清空后缓冲回到 0，tick 自然安静下来。 */
        size_t cap = rb->size - 1;
        size_t offset = (len > cap) ? len - cap : 0;

        rb->iget = 0;
        rb->iput = 0;
        ringbuf_memcpy_put_internal(rb, (const uint8_t *) str + offset, len - offset);
        ctx->notified = false;
    }

    // Notify the host once when the buffer crosses the 50% threshold.
    // Cleared when the host reads, or when the ringbuffer overflows.
    if (!ctx->notified && ringbuf_avail(rb) > (rb->size / 2)) {
        ctx->notified = true;
        ctx->last_notify_ms = mp_hal_ticks_ms();
        omv_protocol_send_event(OMV_PROTOCOL_CHANNEL_ID_STDOUT, OMV_PROTOCOL_EVENT_NOTIFY, false);
    }

    return len;
}

const omv_protocol_channel_t omv_stdin_channel = {
    .priv = &stdio_channel_ctx,
    .id = OMV_PROTOCOL_CHANNEL_ID_STDIN,
    .flags = OMV_PROTOCOL_CHANNEL_FLAG_WRITE | OMV_PROTOCOL_CHANNEL_FLAG_EXEC,
    .name = "stdin",
    .init = stdin_channel_init,
    .poll = stdin_channel_poll,
    .write = stdio_channel_write,
    .ioctl = stdio_channel_ioctl,
    .exec = stdin_channel_exec
};

// Called periodically from the protocol task. If output is queued but the host
// stopped polling, nudge it with a NOTIFY so it issues another CHANNEL_READ.
// 固定 50ms，与 Titan（platform/port/omv_protocol_channel_stdio_titan.c:242）一致。
static void stdout_channel_tick(const omv_protocol_channel_t *channel) {
    stdio_channel_context_t *ctx = channel->priv;
    if (ringbuf_avail(&ctx->ringbuf)
        && (mp_hal_ticks_ms() - ctx->last_notify_ms) >= OMV_PROTOCOL_STDIO_FLUSH_MS) {
        ctx->last_notify_ms = mp_hal_ticks_ms();
        omv_protocol_send_event(OMV_PROTOCOL_CHANNEL_ID_STDOUT, OMV_PROTOCOL_EVENT_NOTIFY, false);
    }
}

const omv_protocol_channel_t omv_stdout_channel = {
    .priv = &stdio_channel_ctx,
    .id = OMV_PROTOCOL_CHANNEL_ID_STDOUT,
    .flags = OMV_PROTOCOL_CHANNEL_FLAG_READ,
    .name = "stdout",
    .init = stdout_channel_init,
    .poll = stdout_channel_poll,
    .size = stdio_channel_size,
    .read = stdio_channel_read,
    .tick = stdout_channel_tick,
};

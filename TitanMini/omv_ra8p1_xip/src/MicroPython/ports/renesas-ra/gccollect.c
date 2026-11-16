/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2013, 2014 Damien P. George
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

#include <stdio.h>
#include <stdint.h>

#include "py/mpstate.h"
#include "py/gc.h"
#include "py/mpthread.h"
#include "shared/runtime/gchelper.h"
#include "shared/runtime/softtimer.h"
#include "gccollect.h"

void gc_collect(void) {

    // start the GC
    gc_collect_start();

    // trace the stack and registers
    gc_helper_collect_regs_and_stack();

    // trace root pointers from any threads
    #if MICROPY_PY_THREAD
    mp_thread_gc_others();
    #endif

    #if MICROPY_PY_PROTOCOL
    // stdin 通道的脚本缓冲分配在 GC 堆上，但它的 vstr_t 位于静态结构体里
    // （ports/ra8/ra8_protocol_channel_stdio.c 的 stdio_channel_ctx），
    // GC 扫不到静态区。不在这里登记成根，下一次回收就会把它当垃圾释放，
    // 之后 vstr_add_len 写的就是已释放内存 —— 堆被踩坏后表现为随机
    // UsageFault（UFSR.UNALIGNED / 停止按钮第二次点击崩溃即此症状）。
    // 对应 Titan platform/micropython/runtime.c:84 的 titan_protocol_gc_collect()。
    extern void omv_stdio_gc_mark(void);
    omv_stdio_gc_mark();
    #endif

    // trace soft timer nodes
    soft_timer_gc_mark_all();

    // end the GC
    gc_collect_end();

}

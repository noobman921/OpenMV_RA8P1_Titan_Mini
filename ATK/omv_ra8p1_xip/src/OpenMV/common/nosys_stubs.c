/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (C) 2024 OpenMV, LLC.
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
 * Stub functions for use when linking with nosys.
 */
#if OMV_NOSYS_STUBS_ENABLE
#include <stdint.h>
#include <errno.h>
#include <sys/stat.h>
#include CMSIS_MCU_H

void _exit(int fd) {
    while (1) {
        __WFI();
    }
}

/* 注意：本文件【有意不定义】_sbrk —— 由 FSP 的
 * ra/fsp/src/bsp/mcu/all/bsp_sbrk.c 提供（它用链接脚本的
 * __end__ / _heap_limit，比这里的实现更贴合本工程的内存布局）。
 * 上游裸机配置下由 nosys_stubs.c 全包；我们是有 FSP 的集成工程。 */

/* _write/_read 必须是本文件提供：
 * 上游实现里它们转发到 board_uart_write/read（TinyUSB hw/bsp/board.c），
 * 但那个接口在本移植上没有实现（board.c 已从构建里移除，否则会引入
 * 未定义的 board_uart_*）。newlib 的 writer.c/readr.c 又确实需要这两个符号，
 * 所以这里给 ENOSYS 版本 —— 与 apriltag 那些调试/文件 I/O 路径一致
 * （它们本就被 APRILTAG_ENABLE_IMAGE_IO/DEBUG = 0 关掉了）。 */
int _write(int handle, char *buffer, int size) {
    (void) handle; (void) buffer; (void) size;
    errno = ENOSYS;
    return -1;
}

int _read(int handle, char *buffer, int size) {
    (void) handle; (void) buffer; (void) size;
    errno = ENOSYS;
    return -1;
}

int _close(int f) {
    errno = ENOSYS;
    return -1;
}

int _lseek(int f, int ptr, int dir) {
    errno = ENOSYS;
    return -1;
}

int _fstat(int fs, struct stat *st) {
    errno = ENOSYS;
    return -1;
}

int _isatty(int f) {
    errno = ENOSYS;
    return 0;
}

int _stat(const char *f, struct stat *st) {
    errno = ENOSYS;
    return -1;
}

#endif // OMV_NOSYS_STUBS_ENABLE

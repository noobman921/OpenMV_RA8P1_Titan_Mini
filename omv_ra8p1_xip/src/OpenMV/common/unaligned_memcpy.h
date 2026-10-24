/*
 * SPDX-License-Identifier: MIT
 *
 * 兼容 shim：上游 1.7.3 把 unaligned_memcpy.{c,h} 改名为 memcpy.{c,h}
 * （函数名 unaligned_memcpy / unaligned_memcpy_rev16 / unaligned_2_to_1_memcpy
 *  完全没变，只有文件名变了）。
 *
 * 本工程自己的文件（ports/ra8/omv_csi.c、common/omv_csi.c 等）仍 include
 * "unaligned_memcpy.h"，这里转发到上游的 memcpy.h，于是：
 *   · 上游文件 include "memcpy.h"        → 正常
 *   · 我们私有文件 include "unaligned_memcpy.h" → 正常
 * 实现只有一份（memcpy.c），不会有重复符号。
 */
#ifndef __UNALIGNED_MEMCPY_H__
#define __UNALIGNED_MEMCPY_H__
#include "memcpy.h"
#endif // __UNALIGNED_MEMCPY_H__

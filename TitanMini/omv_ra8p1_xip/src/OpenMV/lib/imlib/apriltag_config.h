// SPDX-License-Identifier: MIT
//
// Copyright (C) 2025 OpenMV, LLC.
//
// AprilTag configuration for OpenMV embedded targets.
// Included by platform.h when APRILTAG_HAVE_CONFIG is defined.
#ifndef __APRILTAG_CONFIG_H__
#define __APRILTAG_CONFIG_H__
#include <stdint.h>
#include "imlib.h"
#include "umalloc.h"

// Feature disables
#define APRILTAG_ENABLE_PTHREADS        (0)
#define APRILTAG_ENABLE_IMAGE_IO        (0)
#define APRILTAG_ENABLE_DEBUG           (0)
#define APRILTAG_ENABLE_PROFILE         (0)
#define APRILTAG_ENABLE_TAG_NAMES       (0)

#ifndef IMLIB_ENABLE_FINE_APRILTAGS
#define APRILTAG_ENABLE_8_CONNECTIVITY  (0)
#endif

#ifndef IMLIB_ENABLE_HIGH_RES_APRILTAGS
#define APRILTAG_ENABLE_32BIT_UNIONFIND (0)
#endif

// Disable tag families not enabled in imlib_config.h
#ifndef IMLIB_ENABLE_APRILTAGS_TAG16H5
#define APRILTAG_ENABLE_TAG16H5         (0)
#endif
#ifndef IMLIB_ENABLE_APRILTAGS_TAG25H9
#define APRILTAG_ENABLE_TAG25H9         (0)
#endif
#ifndef IMLIB_ENABLE_APRILTAGS_TAG36H10
#define APRILTAG_ENABLE_TAG36H10        (0)
#endif
#ifndef IMLIB_ENABLE_APRILTAGS_TAG36H11
#define APRILTAG_ENABLE_TAG36H11        (0)
#endif
#ifndef IMLIB_ENABLE_APRILTAGS_TAGCIRCLE21H7
#define APRILTAG_ENABLE_TAGCIRCLE21H7   (0)
#endif
#ifndef IMLIB_ENABLE_APRILTAGS_TAGCIRCLE49H12
#define APRILTAG_ENABLE_TAGCIRCLE49H12  (0)
#endif
#ifndef IMLIB_ENABLE_APRILTAGS_TAGCUSTOM48H12
#define APRILTAG_ENABLE_TAGCUSTOM48H12  (0)
#endif
#ifndef IMLIB_ENABLE_APRILTAGS_TAGSTANDARD41H12
#define APRILTAG_ENABLE_TAGSTANDARD41H12 (0)
#endif
#ifndef IMLIB_ENABLE_APRILTAGS_TAGSTANDARD52H13
#define APRILTAG_ENABLE_TAGSTANDARD52H13 (0)
#endif

// Redirect malloc/calloc/realloc/free to UMA allocator.
#define apriltag_malloc(s)      uma_malloc(s, UMA_DTCM)
#define apriltag_calloc(n, s)   uma_calloc((n) * (s), UMA_DTCM)
#define apriltag_realloc(p, s)  uma_realloc(p, s, UMA_DTCM)
#define apriltag_free(p)        uma_free(p)
#define apriltag_assert(x)      ((void) 0)
#define apriltag_poll_events()  imlib_poll_events()

// Simple strtod that avoids pulling in newlib stdio/malloc.
// Only needs to handle small integer constants used in matd_op expressions.
static inline double _apriltag_strtod(const char *s, char **endp) {
    int val = 0;
    while (*s >= '0' && *s <= '9') {
        val = val * 10 + (*s++ - '0');
    }
    if (endp) {
        *endp = (char *) s;
    }
    return (double) val;
}
#define apriltag_strtod(s, endp) _apriltag_strtod(s, endp)

#ifndef APRILTAG_STACK_LIMIT
#define APRILTAG_STACK_LIMIT   (2048)
#endif

/* ptsort 用这个判断"栈上还够不够放临时数组"。
 *
 * 上游默认实现读 `_sstack`（裸机链接脚本的栈底符号）—— 本工程 fsp.ld 里
 * 那句是注释掉的，而且我们跑在 FreeRTOS 任务里，`_sstack` 也不代表当前
 * 任务的栈。Titan 的做法是换成自己的 rt_thread_self() 版本。
 *
 * 这里选择**恒返回 0**（= "栈上没余量，请走堆"）：ptsort 会退回堆分配，
 * 行为正确、只是稍慢一点。理由：
 *   · FreeRTOS 的 uxTaskGetStackHighWaterMark 需要 INCLUDE_uxTaskGetStackHighWaterMark=1，
 *     本工程 FreeRTOSConfig.h:201 是 0，为这一处改动内核配置不划算；
 *   · april_tag 不在热路径上（find_apriltags）。
 * 将来若发现 ptsort 的堆分配成为瓶颈，再把 FreeRTOS 配置打开并换成真实估算。 */
#define apriltag_stack_avail()  ((size_t) 0)

#endif // __APRILTAG_CONFIG_H__

/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (C) 2025 OpenMV, LLC.
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
 * RA8P1 Ethos-U55 NPU glue.
 */
#include <stdio.h>
#include <stdint.h>

#include "hal_data.h"
#include "ethosu_driver.h"
#include "rm_ethosu_api.h"

#include "py/runtime.h"
#include "py/gc.h"
#include "py/nlr.h"
#include "py/mpstate.h"

/* ============================================================================
 * ML 内存分配：模型权重与 TFLM arena 的分工
 * ============================================================================
 * 背景：tflm_backend.cc 拿 gc_info().max_free 当探针尺寸，而那个值是【全堆】最大
 * 连续块——SDRAM 那 2.24MB 区几乎全空，所以探针恒定落在 SDRAM。
 *
 * 探针释放后 gc_free 会把游标重置回第一个区，但 arena 真正分配时若 SRAM 里的死对象
 * 尚未回收，gc_alloc 会【直接跳到后面的区】，不会回头回收 SRAM（见 py/gc.c 的
 * split-heap 扫描结构），于是 arena 静默落进 SDRAM。
 *
 * 实测代价：arena 在 SDRAM 时 NPU 有效带宽 ≈ 76 MB/s，在 SRAM ≈ 194 MB/s；
 * 128x128 BlazeFace 单次推理 73.2 ms vs 41.9 ms。
 *
 * 关键分工（对齐 Titan_Mini_Openmv/platform/npu/titan_ml_memory.c）：
 *   · 权重只读、按需从 flatbuffer 流式读取 —— 放 SDRAM 影响很小；
 *   · arena 装特征图，一次推理要在里面搬几十 MB —— 必须在 SRAM。
 * 两者共用同一个 GC 堆，而 SRAM 只有 1.69MB 装不下「模型 + arena」，
 * 所以必须让大模型【主动让位】，否则 arena 会被挤到 SDRAM。
 *
 * 对策（与 Titan 同构）：
 *   模型侧 omv_ml_model_alloc()：≥128KB 的模型钉到 SDRAM 区；
 *   arena 侧 omv_ml_arena_alloc()：钉游标到第一个区 + 只在这一趟关自动回收，
 *   失败则主动 gc_collect() 后重试。
 *   指针落错区就 m_free 掉按失败处理——宁可退回，也不接受"以为在 SRAM"。
 * -------------------------------------------------------------------------- */

#define OMV_ML_SRAM_START    (0x22000000UL)
#define OMV_ML_SRAM_END      (0x221D4000UL)
#define OMV_ML_SDRAM_START   (0x68000000UL)
#define OMV_ML_SDRAM_LENGTH  (0x02000000UL)
#define OMV_ML_MRAM_START    (0x02000000UL)
#define OMV_ML_MRAM_LENGTH   (0x00100000UL)

/* 小于这个尺寸的模型仍允许进 SRAM：权重本身就小，占不了多少地方，
 * 而 SRAM 上的权重读取更快。阈值与 Titan 一致（titan_ml_memory.c:15）。 */
#define OMV_ML_LARGE_MODEL_BYTES (128U * 1024U)

static int omv_ml_region_contains(uintptr_t base, size_t length, uintptr_t p, size_t size) {
    return p >= base && size <= length && p - base <= length - size;
}

int omv_ml_memory_is_bus_accessible(const void *address, size_t size) {
    uintptr_t p = (uintptr_t) address;
    return omv_ml_region_contains(OMV_ML_SRAM_START, OMV_ML_SRAM_END - OMV_ML_SRAM_START, p, size) ||
           omv_ml_region_contains(OMV_ML_SDRAM_START, OMV_ML_SDRAM_LENGTH, p, size) ||
           omv_ml_region_contains(OMV_ML_MRAM_START, OMV_ML_MRAM_LENGTH, p, size);
}

#if MICROPY_GC_SPLIT_HEAP

typedef struct {
    nlr_jump_callback_node_t node;
    mp_state_mem_area_t *last_free_area;
    uint16_t auto_collect_enabled;
} omv_ml_alloc_guard_t;

// 分配期间可能因 finalizer 抛异常 longjmp 出去；不还原的话游标和自动回收开关
// 会永久停在坏状态。
static void omv_ml_alloc_restore(void *arg) {
    omv_ml_alloc_guard_t *guard = arg;
    MP_STATE_MEM(gc_last_free_area) = guard->last_free_area;
    MP_STATE_MEM(gc_auto_collect_enabled) = guard->auto_collect_enabled;
}

static void *omv_ml_alloc_sram_try(size_t size) {
    if (size > OMV_ML_SRAM_END - OMV_ML_SRAM_START) {
        return NULL;
    }
    omv_ml_alloc_guard_t guard = {
        .last_free_area = MP_STATE_MEM(gc_last_free_area),
        .auto_collect_enabled = MP_STATE_MEM(gc_auto_collect_enabled),
    };
    nlr_push_jump_callback(&guard.node, omv_ml_alloc_restore);
    // 关自动回收会让 gc_alloc 内部的 collected=1，失败时直接返回 NULL 而不自行回收；
    // 回收改由本函数返回后在游标已还原的时机做。
    MP_STATE_MEM(gc_auto_collect_enabled) = 0;
    MP_STATE_MEM(gc_last_free_area) = &MP_STATE_MEM(area);
    // 只把"从哪个区开始"钉住还不够：gc_alloc 还会从该区的
    // gc_last_free_atb_index 开始扫。那个游标随小对象分配单调前移，
    // 一旦越过空闲区起点，本次分配就会看不到前面的空闲块而跳到下一个区。
    // 我们要的是"这个区里有没有足够大的一块"，所以显式从区首扫。
    MP_STATE_MEM(area).gc_last_free_atb_index = 0;
    void *result = m_malloc_maybe(size);
    uintptr_t address = (uintptr_t) result;
    if (result && (address < OMV_ML_SRAM_START || address >= OMV_ML_SRAM_END ||
                   size > OMV_ML_SRAM_END - address)) {
        m_free(result);
        result = NULL;
    }
    nlr_pop_jump_callback(true);
    return result;
}

#endif // MICROPY_GC_SPLIT_HEAP

#if MICROPY_GC_SPLIT_HEAP

static int omv_ml_area_is_sdram(const mp_state_mem_area_t *area) {
    uintptr_t start = (uintptr_t) area->gc_pool_start;
    uintptr_t end = (uintptr_t) area->gc_pool_end;
    return start >= OMV_ML_SDRAM_START && end >= start &&
           end <= OMV_ML_SDRAM_START + OMV_ML_SDRAM_LENGTH;
}

/* 从 GC 的 SDRAM 区里分配，绝不落到 SRAM。
 * 与 omv_ml_alloc_sram_try() 对称：钉住区 + 本趟关自动回收，失败直接返回 NULL。 */
static void *omv_ml_alloc_sdram_try(size_t size) {
    mp_state_mem_area_t *area = &MP_STATE_MEM(area);
    while (area && !omv_ml_area_is_sdram(area)) {
        area = area->next;
    }
    if (!area) {
        return NULL;
    }
    /* gc_alloc 在钉住的区里找不到时会继续往后扫。若后面还有非 SDRAM 区，
     * 大模型就可能漏回 SRAM —— 那正是要避免的，所以干脆放弃。 */
    for (mp_state_mem_area_t *next = area; next; next = next->next) {
        if (!omv_ml_area_is_sdram(next)) {
            return NULL;
        }
    }

    omv_ml_alloc_guard_t guard = {
        .last_free_area = MP_STATE_MEM(gc_last_free_area),
        .auto_collect_enabled = MP_STATE_MEM(gc_auto_collect_enabled),
    };
    nlr_push_jump_callback(&guard.node, omv_ml_alloc_restore);
    MP_STATE_MEM(gc_auto_collect_enabled) = 0;
    MP_STATE_MEM(gc_last_free_area) = area;
    void *result = m_malloc_maybe(size);
    nlr_pop_jump_callback(true);
    return result;
}

#endif // MICROPY_GC_SPLIT_HEAP

/* 模型数据（权重 + 图结构）的分配入口，由 py_ml.c 调用。
 *
 * 大模型必须让出 SRAM：SRAM 只有 1.69MB，装不下「1.25MB 权重 + 0.89MB arena」，
 * 谁先抢到谁留下。而 arena 装特征图、一次推理要搬几十 MB，落到 SDRAM 会慢 2.5 倍；
 * 权重是只读、按需流式读的，放 SDRAM 代价很小。所以由这里强制大模型进 SDRAM，
 * 把 SRAM 完整留给 arena —— 结果不再依赖 GC 游标/碎片/回收时机。 */
void *omv_ml_model_alloc(size_t size) {
    if (size > SIZE_MAX - MICROPY_BYTES_PER_GC_BLOCK) {
        return NULL;
    }
    if (size < OMV_ML_LARGE_MODEL_BYTES) {
        return m_malloc_maybe(size);
    }
#if MICROPY_GC_SPLIT_HEAP
    if (gc_is_locked()) {
        return NULL;
    }
    // 与 gc_alloc 的回收策略保持一致：尊重 gc.disable()，失败时最多回收一次。
    int collected = !MP_STATE_MEM(gc_auto_collect_enabled);
    void *result = omv_ml_alloc_sdram_try(size);
    if (!result && !collected && MP_STATE_MEM(gc_auto_collect_enabled)) {
        gc_collect();
        result = omv_ml_alloc_sdram_try(size);
    }
    return result;
#else
    return m_malloc_maybe(size);
#endif
}

/* ============================================================================
 * TFLM arena 专用静态池
 * ============================================================================
 * 为什么不直接从 GC 堆要：实测（2026-10-22，palm 192×192）collect 之后
 * SRAM 区空闲 1665 KB，但**最大连续段只有 433 KB**，而 arena 需要 866 KB。
 * 几百个活块（合计仅 8.7 KB）把整片空闲切碎了，而 MicroPython 的 GC 没有
 * 压实（compaction）能力，gc_collect() 无法把它们合并回一大块。
 * 任何从 GC 堆申请大块连续内存的做法都要赌堆布局 —— 之前 38ms/379ms
 * 来回跳就是这个原因。
 *
 * 做法：由链接脚本在堆之前预留 1 MB（script/fsp_gen_omv.ld 的
 * .ml_arena_pool），arena 优先从这里切。该段位于 __ram_thread_stack$$ 之前，
 * 所以 _heap_start 自动后移、GC 的 SRAM 堆相应缩小。
 *
 * 池子大小依据见链接脚本里的注释（最大 hand_landmarks 986.6 KB）。
 * 超出则回落到原来的 GC 路径（可能落 SDRAM，功能不受影响）。
 * -------------------------------------------------------------------------- */
extern uint8_t _ml_arena_pool_start[];
extern uint8_t _ml_arena_pool_end[];

static size_t omv_ml_arena_pool_used;
static int omv_ml_arena_pool_refs;

static void *omv_ml_arena_pool_alloc(size_t size) {
    const size_t pool_size = (size_t) (_ml_arena_pool_end - _ml_arena_pool_start);
    if (size == 0 || size > pool_size) {
        return NULL;
    }
    const size_t need = (size + 31u) & ~(size_t) 31u;
    if (omv_ml_arena_pool_used + need > pool_size) {
        // 剩余不够：若当前没有任何未释放的 arena，说明之前的模型都已删除，
        // 整池可以从头再来；否则让调用方走 GC 回退路径。
        if (omv_ml_arena_pool_refs != 0) {
            return NULL;
        }
        omv_ml_arena_pool_used = 0;
    }
    uint8_t *p = _ml_arena_pool_start + omv_ml_arena_pool_used;
    omv_ml_arena_pool_used += need;
    omv_ml_arena_pool_refs++;
    return p;
}

/* 由 ml_backend_deinit_model() 调用（模型对象被回收时）。
 * 只做引用计数，不真的回收单个块 —— 全部释放后下次分配自动从头开始。 */
void omv_ml_arena_free(void *ptr) {
    if (ptr == NULL) {
        return;
    }
    uintptr_t p = (uintptr_t) ptr;
    uintptr_t base = (uintptr_t) _ml_arena_pool_start;
    if (p < base || p >= base + (size_t) (_ml_arena_pool_end - _ml_arena_pool_start)) {
        return;     // 不是池内的指针（GC 回退路径分配的），交给 GC 处理
    }
    if (omv_ml_arena_pool_refs > 0) {
        omv_ml_arena_pool_refs--;
    }
}

void *omv_ml_arena_alloc(size_t size) {
    if (size == 0 || size > SIZE_MAX - MICROPY_BYTES_PER_GC_BLOCK) {
        return NULL;
    }

    // ① 首选静态池：地址固定、必然在 SRAM，不受 GC 堆碎片影响。
    void *pooled = omv_ml_arena_pool_alloc(size);
    if (pooled) {
        return pooled;
    }

    // ② 池子装不下（模型比 1MB 还大）→ 退回原来的 GC 路径。
#if MICROPY_GC_SPLIT_HEAP
    if (!gc_is_locked()) {
        int collected = !MP_STATE_MEM(gc_auto_collect_enabled);
        void *result = omv_ml_alloc_sram_try(size);
        if (!result && !collected && MP_STATE_MEM(gc_auto_collect_enabled)) {
            gc_collect();
            result = omv_ml_alloc_sram_try(size);
        }
        if (result) {
            printf("NPU: arena %u B @ %p (GC SRAM)\n", (unsigned) size, result);
            return result;
        }
    }
#endif
    void *fallback = m_malloc_maybe(size);
    if (fallback) {
        printf("NPU: arena %u B -> %p (SDRAM, slower)\n",
               (unsigned) size, fallback);
    }
    return fallback;
}

// 归档 libtflm-cortex-m55-u55-release.a 里那批平台钩子（ethosu_flush_dcache /
// ethosu_invalidate_dcache / ethosu_irq_handler / ethosu_semaphore_*）都是 weak
// 符号。FSP 的 rm_ethosu 模块已经提供了 cache 维护和中断的强符号版本，会盖住
// 它们，所以这里不需要再覆盖一遍——那样反而会和 FSP 撞成重复定义。

int ra8_npu_init(void) {
    fsp_err_t err = RM_ETHOSU_Open(&g_rm_ethosu0_ctrl, &g_rm_ethosu0_cfg);
    if (FSP_SUCCESS != err) {
        mp_printf(MP_PYTHON_PRINTER, "NPU: RM_ETHOSU_Open failed (%d)\n", (int) err);
        return -1;
    }

    // 这三项必须与 Vela 编译模型时用的 --accelerator-config / 命令流版本一致，
    // 不一致要到 invoke 时才失败，而 RM_ETHOSU_LOG_SEVERITY 默认是关的、看不到
    // 原因，所以在启动时先把真值打出来。
    // 注意 CONFIG.macs_per_cc 是 log2 编码（256 MAC/cc 读出来是 8），
    // Vela 的 --accelerator-config 用的是实际 MAC 数。
    struct ethosu_hw_info hw;
    ethosu_get_hw_info(&g_ethosu0, &hw);
    mp_printf(MP_PYTHON_PRINTER, "NPU: Ethos-U arch %lu.%lu.%lu, MACs/cc=%lu, cmd_stream=%lu\n",
           (unsigned long) hw.version.arch_major_rev,
           (unsigned long) hw.version.arch_minor_rev,
           (unsigned long) hw.version.arch_patch_rev,
           (unsigned long) (1UL << hw.cfg.macs_per_cc),
           (unsigned long) hw.cfg.cmd_stream_version);
    return 0;
}

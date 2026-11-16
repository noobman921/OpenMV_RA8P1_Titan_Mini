/*
                  Linker File for Renesas FSP
*/

INCLUDE memory_regions_sec.ld
INCLUDE fsp_gen_omv.ld
/* INCLUDE fsp_gen.ld */

_etext      = __flash_readonly$$Limit;
_sidata     = __ram_from_flash$$Load;
_ram_start  = RAM_START;
_sdata      = __ram_from_flash$$Base;
_edata      = __ram_from_flash$$Limit;
_sbss       = __ram_zero$$Base;
_ebss       = __ram_zero$$Limit;
_heap_start = __ram_thread_stack$$Limit;
__MP_RAM_END = RAM_START + RAM_LENGTH;
_ram_end    = __MP_RAM_END;

/* OpenMV 图像通路落 SDRAM，内部 RAM 全部让给 GC 堆。
 *
 * 本文件只做 INCLUDE + 符号赋值；MEMORY 区域和输出段在
 * memory_regions_sec.ld（RASC 生成）和 fsp_gen_omv.ld（接管 fsp_gen.ld 的分叉）。
 *
 * 完整 SDRAM 布局（0x68000000 起）：
 *   0x68000000 .. 0x681C2000   VIN 三帧采集缓冲（.sdram_noinit_nocache，MPU 标 nocache）
 *   0x681C2000 .. 0x68600000   GC 第二堆 4.24 MB（omvmain.c 的第一个 region）
 *   0x68600000 .. 0x68680000   IDE 流缓冲 SB     512 KB （_sb_memory_*）
 *   0x68680000 .. 0x68800000   GC 第三堆 2 MB   （omvmain.c 的第二个 region）
 *   0x68800000 .. 0x68A00000   OMV_CODE          2 MB   （imlib/ulab/py_image/frozen）
 *   0x68A00000 .. 0x69200000   UMA 主池          8 MB   （_uma_pool_*）
 *   0x69200000 .. 0x6A000000   GC 第四堆 14 MB  （omvmain.c 的第三个 region）
 *
 * 另有一个 64 KB 的 UMA fast 池不在这里：它是 C 数组
 * （omvmain.c:511，段 .dtcm_noinit），落在 DTCM 0x20000000..0x20010000。
 */
/* IDE 流缓冲：framebuffer.c 的 FB_STREAM 用它，位于 GC 第二堆上界之上。 */
_sb_memory_start   = 0x68600000;
_sb_memory_end     = _sb_memory_start + 0x00080000;   /* 512 KB */

/* UMA 主池：TLSF 运行期接管，链接期只需地址范围。
 * 起始点必须 >= OMV_CODE 上沿；fsp_gen_omv.ld 末尾有 ASSERT 守着这条。 */
_uma_pool_start    = 0x68A00000;
_uma_pool_end      = _uma_pool_start + 0x00800000;    /* 8 MB */

/* GC 主堆吃满内部 RAM 剩余部分。 */
_heap_end = __MP_RAM_END;
/*
_sstack     = g_main_stack;
_estack     = g_main_stack + 0x1000;
*/

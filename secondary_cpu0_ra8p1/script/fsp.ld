/*
                  Linker File for Renesas FSP
*/

INCLUDE memory_regions_sec.ld
INCLUDE fsp_gen.ld

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

/* OpenMV framebuffer: reserve top 1MB of RAM
 *   Main FB  : 640KB (VGA 640x480 RGB565 = 614400B single frame)
 *   Stream SB: 256KB (IDE streaming buffer - must fit VGA JPEG ~200KB)
 *   fb_alloc : 128KB (image processing temporary allocations)
 *   Total    : 1MB = 0x100000  (heap reduced from 768KB to 512KB)
 */
__omv_region_start = __MP_RAM_END - 0x100000;
_heap_end          = __omv_region_start;

_fb_memory_start   = __omv_region_start;
_fb_memory_end     = __omv_region_start + 0xA0000;
_sb_memory_start   = _fb_memory_end;
_sb_memory_end     = __omv_region_start + 0xE0000;
_fb_alloc_end      = __MP_RAM_END - 0x20000;
_fballoc_overlay_end = __MP_RAM_END;
_fballoc_overlay_start = _fb_alloc_end;
/*
_sstack     = g_main_stack;
_estack     = g_main_stack + 0x1000;
*/

/* ============================================================================
 * 本文件是 RASC 生成的 Debug/fsp_gen.ld 的项目自持分叉（B3：接管 linker script）。
 *
 * 由 script/fsp.ld 用 INCLUDE 引入，替代原来的 `INCLUDE fsp_gen.ld`。
 * 唯一实质改动：__itcm_from_flash$$ 增加了按函数名抓取的 ITCM 热函数白名单
 * 与 ECC 对齐，并在文件末尾加了三条断言（搜索 "OMV-ITCM" 定位）。
 *
 * ⚠️ 维护成本：RASC 重新生成 Debug/fsp_gen.ld 时（改外设/引脚/内存配置），
 *    本文件**不会**自动同步。需要人工 diff 生成文件并把差异合并过来。
 *    若无法接受，改回 `INCLUDE fsp_gen.ld` + objcopy 改名（路线 B2）。
 * ========================================================================= */
MEMORY
{
    RAM (rwx) : ORIGIN = RAM_START, LENGTH = RAM_LENGTH
    FLASH (rx) : ORIGIN = FLASH_START, LENGTH = FLASH_LENGTH
    DATA_FLASH (rx) : ORIGIN = DATA_FLASH_START, LENGTH = DATA_FLASH_LENGTH
    /* ---- OMV-VISION ----------------------------------------------------
     * SDRAM 只放 VIN 采集缓冲，上界必须卡死在 0x681C2000 —— 那是
     * omvmain.c:224 硬编码的 GC 第二堆起点。写精确值，让链接器在
     * 越界时直接报 "section will not fit"，而不是运行期总线错误。
     * -------------------------------------------------------------------- */
    SDRAM (rwx) : ORIGIN = SDRAM_START, LENGTH = 0x001C2000
    /* imlib 代码执行区（对标 Titan 的 VISION 段）。
     * 起于 0x68800000 = fb_alloc overlay 上沿（script/fsp.ld:28）。
     * 2 MB：imlib(456K) + ulab(181K) + py_image(57K) + frozen(12K) ≈ 706K，
     * 留余量给后续把 py/ 也搬进来。上界 0x68A00000 仍在 fsp.ld 标注的预留区内
     * （0x68800000..0x69A00000），不碰 FB 池/SB/fb_alloc。 */
    OMV_CODE (rx) : ORIGIN = 0x68800000, LENGTH = 0x00200000
    /* UMA 主池不在这里声明 —— 它是运行期托管内存，只需 fsp.ld 里的
     * _uma_pool_start/_end 两个符号（与 _sb_memory_* 同一风格）。 */
    OSPI0_CS0 (rwx) : ORIGIN = OSPI0_CS0_START, LENGTH = OSPI0_CS0_LENGTH
    OSPI0_CS1 (rx) : ORIGIN = OSPI0_CS1_START, LENGTH = OSPI0_CS1_LENGTH
    OSPI1_CS0 (rwx) : ORIGIN = OSPI1_CS0_START, LENGTH = OSPI1_CS0_LENGTH
    OSPI1_CS1 (rx) : ORIGIN = OSPI1_CS1_START, LENGTH = OSPI1_CS1_LENGTH
    OPTION_SETTING_OFS0 (r) : ORIGIN = OPTION_SETTING_OFS0_START, LENGTH = OPTION_SETTING_OFS0_LENGTH
    OPTION_SETTING_OFS2 (r) : ORIGIN = OPTION_SETTING_OFS2_START, LENGTH = OPTION_SETTING_OFS2_LENGTH
    OPTION_SETTING_SAS (r) : ORIGIN = OPTION_SETTING_SAS_START, LENGTH = OPTION_SETTING_SAS_LENGTH
    OPTION_SETTING_OFS1 (r) : ORIGIN = OPTION_SETTING_OFS1_START, LENGTH = OPTION_SETTING_OFS1_LENGTH
    OPTION_SETTING_OFS1_SEC (r) : ORIGIN = OPTION_SETTING_OFS1_SEC_START, LENGTH = OPTION_SETTING_OFS1_SEC_LENGTH
    OPTION_SETTING_OFS1_SEL (r) : ORIGIN = OPTION_SETTING_OFS1_SEL_START, LENGTH = OPTION_SETTING_OFS1_SEL_LENGTH
    OPTION_SETTING_OFS3 (r) : ORIGIN = OPTION_SETTING_OFS3_START, LENGTH = OPTION_SETTING_OFS3_LENGTH
    OPTION_SETTING_OFS3_SEC (r) : ORIGIN = OPTION_SETTING_OFS3_SEC_START, LENGTH = OPTION_SETTING_OFS3_SEC_LENGTH
    OPTION_SETTING_OFS3_SEL (r) : ORIGIN = OPTION_SETTING_OFS3_SEL_START, LENGTH = OPTION_SETTING_OFS3_SEL_LENGTH
    OPTION_SETTING_BPS (r) : ORIGIN = OPTION_SETTING_BPS_START, LENGTH = OPTION_SETTING_BPS_LENGTH
    OPTION_SETTING_BPS_SEC (r) : ORIGIN = OPTION_SETTING_BPS_SEC_START, LENGTH = OPTION_SETTING_BPS_SEC_LENGTH
    OPTION_SETTING_OTP_FSBLCTRL0 (r) : ORIGIN = OPTION_SETTING_OTP_FSBLCTRL0_START, LENGTH = OPTION_SETTING_OTP_FSBLCTRL0_LENGTH
    OPTION_SETTING_OTP_FSBLCTRL1 (r) : ORIGIN = OPTION_SETTING_OTP_FSBLCTRL1_START, LENGTH = OPTION_SETTING_OTP_FSBLCTRL1_LENGTH
    OPTION_SETTING_OTP_FSBLCTRL2 (r) : ORIGIN = OPTION_SETTING_OTP_FSBLCTRL2_START, LENGTH = OPTION_SETTING_OTP_FSBLCTRL2_LENGTH
    OPTION_SETTING_OTP_SAMR (r) : ORIGIN = OPTION_SETTING_OTP_SAMR_START, LENGTH = OPTION_SETTING_OTP_SAMR_LENGTH
    OPTION_SETTING_OTP_SACC00 (r) : ORIGIN = OPTION_SETTING_OTP_SACC00_START, LENGTH = OPTION_SETTING_OTP_SACC00_LENGTH
    OPTION_SETTING_OTP_SACC10 (r) : ORIGIN = OPTION_SETTING_OTP_SACC10_START, LENGTH = OPTION_SETTING_OTP_SACC10_LENGTH
    OPTION_SETTING_OTP_SACC01 (r) : ORIGIN = OPTION_SETTING_OTP_SACC01_START, LENGTH = OPTION_SETTING_OTP_SACC01_LENGTH
    OPTION_SETTING_OTP_SACC11 (r) : ORIGIN = OPTION_SETTING_OTP_SACC11_START, LENGTH = OPTION_SETTING_OTP_SACC11_LENGTH
    OPTION_SETTING_OTP_SACC02 (r) : ORIGIN = OPTION_SETTING_OTP_SACC02_START, LENGTH = OPTION_SETTING_OTP_SACC02_LENGTH
    OPTION_SETTING_OTP_SACC12 (r) : ORIGIN = OPTION_SETTING_OTP_SACC12_START, LENGTH = OPTION_SETTING_OTP_SACC12_LENGTH
    OPTION_SETTING_OTP_SACC03 (r) : ORIGIN = OPTION_SETTING_OTP_SACC03_START, LENGTH = OPTION_SETTING_OTP_SACC03_LENGTH
    OPTION_SETTING_OTP_SACC13 (r) : ORIGIN = OPTION_SETTING_OTP_SACC13_START, LENGTH = OPTION_SETTING_OTP_SACC13_LENGTH
    OPTION_SETTING_OTP_PBPS_SEC (r) : ORIGIN = OPTION_SETTING_OTP_PBPS_SEC_START, LENGTH = OPTION_SETTING_OTP_PBPS_SEC_LENGTH
    OPTION_SETTING_OTP_PBPS (r) : ORIGIN = OPTION_SETTING_OTP_PBPS_START, LENGTH = OPTION_SETTING_OTP_PBPS_LENGTH
    OPTION_SETTING_OTP_ZHUK (r) : ORIGIN = OPTION_SETTING_OTP_ZHUK_START, LENGTH = OPTION_SETTING_OTP_ZHUK_LENGTH
    ITCM (rwx) : ORIGIN = ITCM_START, LENGTH = ITCM_LENGTH
    DTCM (rwx) : ORIGIN = DTCM_START, LENGTH = DTCM_LENGTH
}

/* code entry point...need to define to keep crt0 _start out */
ENTRY( Reset_Handler)
/* Library configurations */
GROUP(libgcc.a libc.a libm.a)

SECTIONS
{
    /***** OSPI0_CS1 memory section allocations ******/
    .ospi0_cs1.startof (READONLY) :
    {
        __ddsc_OSPI0_CS1_START = .;

    }> OSPI0_CS1
    /***** SDRAM memory section allocations ******/
    .sdram.startof :
    {
        __ddsc_SDRAM_START = .;

    }> SDRAM
    /* sdram initialized from ospi0_cs1 */
    __sdram_from_ospi0_cs1$$ :
    {
        __sdram_from_ospi0_cs1$$Base = .;__sdram_from_ospi0_cs1$$Load = LOADADDR(__sdram_from_ospi0_cs1$$);
        /* section.sdram.from_ospi0_cs1 */
        *(.sdram_from_ospi0_cs1)
        /* section.sdram.code_from_ospi0_cs1 */
        *(.sdram_code_from_ospi0_cs1)
        __sdram_from_ospi0_cs1$$Limit = .;
    }> SDRAM AT > OSPI0_CS1

    /***** OSPI0_CS0 memory section allocations ******/
    .ospi0_cs0.startof :
    {
        __ddsc_OSPI0_CS0_START = .;

    }> OSPI0_CS0
    /* ospi0_cs0 initialized from ospi0_cs1 */
    __ospi0_cs0_from_ospi0_cs1$$ :
    {
        __ospi0_cs0_from_ospi0_cs1$$Base = .;__ospi0_cs0_from_ospi0_cs1$$Load = LOADADDR(__ospi0_cs0_from_ospi0_cs1$$);
        /* section.ospi0_cs0.from_ospi0_cs1 */
        *(.ospi0_cs0_from_ospi0_cs1)
        /* section.ospi0_cs0.code_from_ospi0_cs1 */
        *(.ospi0_cs0_code_from_ospi0_cs1)
        __ospi0_cs0_from_ospi0_cs1$$Limit = .;
    }> OSPI0_CS0 AT > OSPI0_CS1

    /***** OSPI1_CS0 memory section allocations ******/
    .ospi1_cs0.startof :
    {
        __ddsc_OSPI1_CS0_START = .;

    }> OSPI1_CS0
    /* ospi1_cs0 initialized from ospi0_cs1 */
    __ospi1_cs0_from_ospi0_cs1$$ :
    {
        __ospi1_cs0_from_ospi0_cs1$$Base = .;__ospi1_cs0_from_ospi0_cs1$$Load = LOADADDR(__ospi1_cs0_from_ospi0_cs1$$);
        /* section.ospi1_cs0.from_ospi0_cs1 */
        *(.ospi1_cs0_from_ospi0_cs1)
        /* section.ospi1_cs0.code_from_ospi0_cs1 */
        *(.ospi1_cs0_code_from_ospi0_cs1)
        __ospi1_cs0_from_ospi0_cs1$$Limit = .;
    }> OSPI1_CS0 AT > OSPI0_CS1

    /***** ITCM memory section allocations ******/
    .itcm.startof :
    {
        __ddsc_ITCM_START = .;

    }> ITCM
    /* itcm initialized from ospi0_cs1 */
    __itcm_from_ospi0_cs1$$ :
    {
        __itcm_from_ospi0_cs1$$Base = .;__itcm_from_ospi0_cs1$$Load = LOADADDR(__itcm_from_ospi0_cs1$$);
        /* section.itcm.from_ospi0_cs1 */
        *(.itcm_from_ospi0_cs1)
        /* section.itcm.code_from_ospi0_cs1 */
        *(.itcm_code_from_ospi0_cs1)
        __itcm_from_ospi0_cs1$$Limit = .;
    }> ITCM AT > OSPI0_CS1

    /***** DTCM memory section allocations ******/
    .dtcm.startof :
    {
        __ddsc_DTCM_START = .;

    }> DTCM
    /* dtcm initialized from ospi0_cs1 */
    __dtcm_from_ospi0_cs1$$ :
    {
        __dtcm_from_ospi0_cs1$$Base = .;__dtcm_from_ospi0_cs1$$Load = LOADADDR(__dtcm_from_ospi0_cs1$$);
        /* section.dtcm.from_ospi0_cs1 */
        *(.dtcm_from_ospi0_cs1)
        /* section.dtcm.code_from_ospi0_cs1 */
        *(.dtcm_code_from_ospi0_cs1)
        __dtcm_from_ospi0_cs1$$Limit = .;
    }> DTCM AT > OSPI0_CS1

    /***** RAM memory section allocations ******/
    .ram.startof :
    {
        __ddsc_RAM_START = .;

    }> RAM
    __ram_dtc_vector$$ (NOLOAD) :
    {
        __ram_dtc_vector$$Base = .;
        *(.fsp_dtc_vector_table)
        __ram_dtc_vector$$Limit = .;
    }> RAM
    /* ram initialized from ospi0_cs1 */
    __ram_from_ospi0_cs1$$ :
    {
        __ram_from_ospi0_cs1$$Base = .;__ram_from_ospi0_cs1$$Load = LOADADDR(__ram_from_ospi0_cs1$$);
        /* section.ram.from_ospi0_cs1 */
        *(.ram_from_ospi0_cs1)
        /* section.ram.code_from_ospi0_cs1 */
        *(.ram_code_from_ospi0_cs1)
        __ram_from_ospi0_cs1$$Limit = .;
    }> RAM AT > OSPI0_CS1

    __ospi0_cs1_readonly$$ (READONLY) :
    {
        __ospi0_cs1_readonly$$Base = .;
        /* section.ospi0_cs1.readonly */
        *(.ospi0_cs1)
        /* section.ospi0_cs1.code */
        *(.ospi0_cs1_code)
        __ospi0_cs1_readonly$$Limit = .;
    }> OSPI0_CS1
    __ospi0_cs1_noinit$$ (NOLOAD) :
    {
        __ospi0_cs1_noinit$$Base = .;
        /* section.ospi0_cs1.noinit */
        *(.ospi0_cs1_noinit)
        __ospi0_cs1_noinit$$Limit = .;
    }> OSPI0_CS1
    .ospi0_cs1.endof ALIGN(.,512) (READONLY) :
    {
        __ddsc_OSPI0_CS1_END = .;

    }> OSPI0_CS1

    /***** OSPI1_CS1 memory section allocations ******/
    .ospi1_cs1.startof (READONLY) :
    {
        __ddsc_OSPI1_CS1_START = .;

    }> OSPI1_CS1
    /***** SDRAM memory section allocations ******/
    /* sdram initialized from ospi1_cs1 */
    __sdram_from_ospi1_cs1$$ :
    {
        __sdram_from_ospi1_cs1$$Base = .;__sdram_from_ospi1_cs1$$Load = LOADADDR(__sdram_from_ospi1_cs1$$);
        /* section.sdram.from_ospi1_cs1 */
        *(.sdram_from_ospi1_cs1)
        /* section.sdram.code_from_ospi1_cs1 */
        *(.sdram_code_from_ospi1_cs1)
        __sdram_from_ospi1_cs1$$Limit = .;
    }> SDRAM AT > OSPI1_CS1

    /***** OSPI0_CS0 memory section allocations ******/
    /* ospi0_cs0 initialized from ospi1_cs1 */
    __ospi0_cs0_from_ospi1_cs1$$ :
    {
        __ospi0_cs0_from_ospi1_cs1$$Base = .;__ospi0_cs0_from_ospi1_cs1$$Load = LOADADDR(__ospi0_cs0_from_ospi1_cs1$$);
        /* section.ospi0_cs0.from_ospi1_cs1 */
        *(.ospi0_cs0_from_ospi1_cs1)
        /* section.ospi0_cs0.code_from_ospi1_cs1 */
        *(.ospi0_cs0_code_from_ospi1_cs1)
        __ospi0_cs0_from_ospi1_cs1$$Limit = .;
    }> OSPI0_CS0 AT > OSPI1_CS1

    /***** OSPI1_CS0 memory section allocations ******/
    /* ospi1_cs0 initialized from ospi1_cs1 */
    __ospi1_cs0_from_ospi1_cs1$$ :
    {
        __ospi1_cs0_from_ospi1_cs1$$Base = .;__ospi1_cs0_from_ospi1_cs1$$Load = LOADADDR(__ospi1_cs0_from_ospi1_cs1$$);
        /* section.ospi1_cs0.from_ospi1_cs1 */
        *(.ospi1_cs0_from_ospi1_cs1)
        /* section.ospi1_cs0.code_from_ospi1_cs1 */
        *(.ospi1_cs0_code_from_ospi1_cs1)
        __ospi1_cs0_from_ospi1_cs1$$Limit = .;
    }> OSPI1_CS0 AT > OSPI1_CS1

    /***** ITCM memory section allocations ******/
    /* itcm initialized from ospi1_cs1 */
    __itcm_from_ospi1_cs1$$ :
    {
        __itcm_from_ospi1_cs1$$Base = .;__itcm_from_ospi1_cs1$$Load = LOADADDR(__itcm_from_ospi1_cs1$$);
        /* section.itcm.from_ospi1_cs1 */
        *(.itcm_from_ospi1_cs1)
        /* section.itcm.code_from_ospi1_cs1 */
        *(.itcm_code_from_ospi1_cs1)
        __itcm_from_ospi1_cs1$$Limit = .;
    }> ITCM AT > OSPI1_CS1

    /***** DTCM memory section allocations ******/
    /* dtcm initialized from ospi1_cs1 */
    __dtcm_from_ospi1_cs1$$ :
    {
        __dtcm_from_ospi1_cs1$$Base = .;__dtcm_from_ospi1_cs1$$Load = LOADADDR(__dtcm_from_ospi1_cs1$$);
        /* section.dtcm.from_ospi1_cs1 */
        *(.dtcm_from_ospi1_cs1)
        /* section.dtcm.code_from_ospi1_cs1 */
        *(.dtcm_code_from_ospi1_cs1)
        __dtcm_from_ospi1_cs1$$Limit = .;
    }> DTCM AT > OSPI1_CS1

    /***** RAM memory section allocations ******/
    /* ram initialized from ospi1_cs1 */
    __ram_from_ospi1_cs1$$ :
    {
        __ram_from_ospi1_cs1$$Base = .;__ram_from_ospi1_cs1$$Load = LOADADDR(__ram_from_ospi1_cs1$$);
        /* section.ram.from_ospi1_cs1 */
        *(.ram_from_ospi1_cs1)
        /* section.ram.code_from_ospi1_cs1 */
        *(.ram_code_from_ospi1_cs1)
        __ram_from_ospi1_cs1$$Limit = .;
    }> RAM AT > OSPI1_CS1

    __ospi1_cs1_readonly$$ (READONLY) :
    {
        __ospi1_cs1_readonly$$Base = .;
        /* section.ospi1_cs1.readonly */
        *(.ospi1_cs1)
        /* section.ospi1_cs1.code */
        *(.ospi1_cs1_code)
        __ospi1_cs1_readonly$$Limit = .;
    }> OSPI1_CS1
    __ospi1_cs1_noinit$$ (NOLOAD) :
    {
        __ospi1_cs1_noinit$$Base = .;
        /* section.ospi1_cs1.noinit */
        *(.ospi1_cs1_noinit)
        __ospi1_cs1_noinit$$Limit = .;
    }> OSPI1_CS1
    .ospi1_cs1.endof ALIGN(.,512) (READONLY) :
    {
        __ddsc_OSPI1_CS1_END = .;

    }> OSPI1_CS1

    /***** DATA_FLASH memory section allocations ******/
    .data_flash.startof (READONLY) :
    {
        __ddsc_DATA_FLASH_START = .;

    }> DATA_FLASH
    /***** SDRAM memory section allocations ******/
    /* sdram initialized from data_flash */
    __sdram_from_data_flash$$ :
    {
        __sdram_from_data_flash$$Base = .;__sdram_from_data_flash$$Load = LOADADDR(__sdram_from_data_flash$$);
        /* section.sdram.from_data_flash */
        *(.sdram_from_data_flash)
        /* section.sdram.code_from_data_flash */
        *(.sdram_code_from_data_flash)
        __sdram_from_data_flash$$Limit = .;
    }> SDRAM AT > DATA_FLASH

    /***** OSPI0_CS0 memory section allocations ******/
    /* ospi0_cs0 initialized from data_flash */
    __ospi0_cs0_from_data_flash$$ :
    {
        __ospi0_cs0_from_data_flash$$Base = .;__ospi0_cs0_from_data_flash$$Load = LOADADDR(__ospi0_cs0_from_data_flash$$);
        /* section.ospi0_cs0.from_data_flash */
        *(.ospi0_cs0_from_data_flash)
        /* section.ospi0_cs0.code_from_data_flash */
        *(.ospi0_cs0_code_from_data_flash)
        __ospi0_cs0_from_data_flash$$Limit = .;
    }> OSPI0_CS0 AT > DATA_FLASH

    /***** OSPI1_CS0 memory section allocations ******/
    /* ospi1_cs0 initialized from data_flash */
    __ospi1_cs0_from_data_flash$$ :
    {
        __ospi1_cs0_from_data_flash$$Base = .;__ospi1_cs0_from_data_flash$$Load = LOADADDR(__ospi1_cs0_from_data_flash$$);
        /* section.ospi1_cs0.from_data_flash */
        *(.ospi1_cs0_from_data_flash)
        /* section.ospi1_cs0.code_from_data_flash */
        *(.ospi1_cs0_code_from_data_flash)
        __ospi1_cs0_from_data_flash$$Limit = .;
    }> OSPI1_CS0 AT > DATA_FLASH

    /***** ITCM memory section allocations ******/
    /* itcm initialized from data_flash */
    __itcm_from_data_flash$$ :
    {
        __itcm_from_data_flash$$Base = .;__itcm_from_data_flash$$Load = LOADADDR(__itcm_from_data_flash$$);
        /* section.itcm.from_data_flash */
        *(.itcm_from_data_flash)
        /* section.itcm.code_from_data_flash */
        *(.itcm_code_from_data_flash)
        __itcm_from_data_flash$$Limit = .;
    }> ITCM AT > DATA_FLASH

    /***** DTCM memory section allocations ******/
    /* dtcm initialized from data_flash */
    __dtcm_from_data_flash$$ :
    {
        __dtcm_from_data_flash$$Base = .;__dtcm_from_data_flash$$Load = LOADADDR(__dtcm_from_data_flash$$);
        /* section.dtcm.from_data_flash */
        *(.dtcm_from_data_flash)
        /* section.dtcm.code_from_data_flash */
        *(.dtcm_code_from_data_flash)
        __dtcm_from_data_flash$$Limit = .;
    }> DTCM AT > DATA_FLASH

    /***** RAM memory section allocations ******/
    /* ram initialized from data_flash */
    __ram_from_data_flash$$ :
    {
        __ram_from_data_flash$$Base = .;__ram_from_data_flash$$Load = LOADADDR(__ram_from_data_flash$$);
        /* section.ram.from_data_flash */
        *(.ram_from_data_flash)
        /* section.ram.code_from_data_flash */
        *(.ram_code_from_data_flash)
        __ram_from_data_flash$$Limit = .;
    }> RAM AT > DATA_FLASH

    __data_flash_readonly$$ (READONLY) :
    {
        __data_flash_readonly$$Base = .;
        /* section.data_flash.readonly */
        *(.data_flash)
        /* section.data_flash.code */
        *(.data_flash_code)
        __data_flash_readonly$$Limit = .;
    }> DATA_FLASH
    __data_flash_noinit$$ (NOLOAD) :
    {
        __data_flash_noinit$$Base = .;
        /* section.data_flash.noinit */
        *(.data_flash_noinit)
        __data_flash_noinit$$Limit = .;
    }> DATA_FLASH
    .data_flash.endof ALIGN(.,512) (READONLY) :
    {
        __ddsc_DATA_FLASH_END = .;

    }> DATA_FLASH

    /***** FLASH memory section allocations ******/
    .flash.startof (READONLY) :
    {
        __ddsc_FLASH_START = .;

    }> FLASH
    /* MCU vector table */
    __flash_vectors$$ (READONLY) :
    {
        __flash_vectors$$Base = .; _VECTORS = .;
        KEEP(*(.fixed_vectors))
        KEEP(*(.application_vectors))
        __flash_vectors$$Limit = .;
    }> FLASH
    __flash_noinit$$ (NOLOAD) :
    {
        __flash_noinit$$Base = .;
        /* section.flash.noinit */
        *(.flash_noinit)
        __flash_noinit$$Limit = .;
    }> FLASH
    /***** SDRAM memory section allocations ******/
    /* sdram initialized from flash */
    __sdram_from_flash$$ :
    {
        __sdram_from_flash$$Base = .;__sdram_from_flash$$Load = LOADADDR(__sdram_from_flash$$);
        /* 图像处理：预处理/后处理的主力 */
        *lib/imlib/*.o(.text .text.* .rodata .rodata.*)
        /* ulab：*= / += / take / reshape 等原地运算，预处理与后处理都重度使用 */
        *modules/ulab/*.o(.text .text.* .rodata .rodata.*)
        /* to_ndarray：RGB565 -> float32 的转换（py_image.c） */
        *modules/py_image.o(.text .text.* .rodata .rodata.*)
        /* 冻结的 Python 字节码（ml 包等），取指密集 */
        *ports/renesas-ra/frozen_content.o(.text .text.* .rodata .rodata.*)
        *src/MicroPython/*.o(.text .text.* .rodata .rodata.*)
        *src/OpenMV/*.o(.text .text.* .rodata .rodata.*)
        /* section.sdram.from_flash */
        *(.sdram_from_flash)
        /* section.sdram.code_from_flash */
        *(.sdram_code_from_flash)
        __sdram_from_flash$$Limit = .;
    }> OMV_CODE AT > FLASH
    /* Non-initialized, non-cached sdram */
    __sdram_noinit_nocache$$ (NOLOAD) : ALIGN(32)
    {
        __sdram_noinit_nocache$$Base = .;
        /* section.sdram.noinit_nocache */
        *(.sdram_noinit_nocache)
        __sdram_noinit_nocache$$Limit = .;
    }> SDRAM
    /* Zeroed, non-cached sdram */
    __sdram_zero_nocache$$ (NOLOAD) :
    {
        __sdram_zero_nocache$$Base = .;
        /* section.sdram.zero_nocache */
        *(.sdram_nocache)
        . = ALIGN(32);
        __sdram_zero_nocache$$Limit = .;
    }> SDRAM
    /* Non-initialized sdram */
    __sdram_noinit$$ (NOLOAD) :
    {
        __sdram_noinit$$Base = .;
        /* section.sdram.noinit */
        *(.sdram_noinit)
        __sdram_noinit$$Limit = .;
    }> SDRAM
    /* Zeroed sdram */
    __sdram_zero$$ (NOLOAD) :
    {
        __sdram_zero$$Base = .;
        /* section.sdram.zero */
        *(.sdram)
        __sdram_zero$$Limit = .;
    }> SDRAM
    .sdram.endof ALIGN(.,512) :
    {
        __ddsc_SDRAM_END = .;

    }> SDRAM

    /***** OSPI0_CS0 memory section allocations ******/
    /* ospi0_cs0 initialized from flash */
    __ospi0_cs0_from_flash$$ :
    {
        __ospi0_cs0_from_flash$$Base = .;__ospi0_cs0_from_flash$$Load = LOADADDR(__ospi0_cs0_from_flash$$);
        /* section.ospi0_cs0.from_flash */
        *(.ospi0_cs0_from_flash)
        /* section.ospi0_cs0.code_from_flash */
        *(.ospi0_cs0_code_from_flash)
        __ospi0_cs0_from_flash$$Limit = .;
    }> OSPI0_CS0 AT > FLASH
    /* Non-initialized, non-cached ospi0_cs0 */
    __ospi0_cs0_noinit_nocache$$ (NOLOAD) : ALIGN(32)
    {
        __ospi0_cs0_noinit_nocache$$Base = .;
        /* section.ospi0_cs0.noinit_nocache */
        *(.ospi0_cs0_noinit_nocache)
        __ospi0_cs0_noinit_nocache$$Limit = .;
    }> OSPI0_CS0
    /* Zeroed, non-cached ospi0_cs0 */
    __ospi0_cs0_zero_nocache$$ (NOLOAD) :
    {
        __ospi0_cs0_zero_nocache$$Base = .;
        /* section.ospi0_cs0.zero_nocache */
        *(.ospi0_cs0_nocache)
        . = ALIGN(32);
        __ospi0_cs0_zero_nocache$$Limit = .;
    }> OSPI0_CS0
    /* Non-initialized ospi0_cs0 */
    __ospi0_cs0_noinit$$ (NOLOAD) :
    {
        __ospi0_cs0_noinit$$Base = .;
        /* section.ospi0_cs0.noinit */
        *(.ospi0_cs0_noinit)
        __ospi0_cs0_noinit$$Limit = .;
    }> OSPI0_CS0
    /* Zeroed ospi0_cs0 */
    __ospi0_cs0_zero$$ (NOLOAD) :
    {
        __ospi0_cs0_zero$$Base = .;
        /* section.ospi0_cs0.zero */
        *(.ospi0_cs0)
        __ospi0_cs0_zero$$Limit = .;
    }> OSPI0_CS0
    .ospi0_cs0.endof ALIGN(.,512) :
    {
        __ddsc_OSPI0_CS0_END = .;

    }> OSPI0_CS0

    /***** OSPI1_CS0 memory section allocations ******/
    /* ospi1_cs0 initialized from flash */
    __ospi1_cs0_from_flash$$ :
    {
        __ospi1_cs0_from_flash$$Base = .;__ospi1_cs0_from_flash$$Load = LOADADDR(__ospi1_cs0_from_flash$$);
        /* section.ospi1_cs0.from_flash */
        *(.ospi1_cs0_from_flash)
        /* section.ospi1_cs0.code_from_flash */
        *(.ospi1_cs0_code_from_flash)
        __ospi1_cs0_from_flash$$Limit = .;
    }> OSPI1_CS0 AT > FLASH
    /* Non-initialized, non-cached ospi1_cs0 */
    __ospi1_cs0_noinit_nocache$$ (NOLOAD) : ALIGN(32)
    {
        __ospi1_cs0_noinit_nocache$$Base = .;
        /* section.ospi1_cs0.noinit_nocache */
        *(.ospi1_cs0_noinit_nocache)
        __ospi1_cs0_noinit_nocache$$Limit = .;
    }> OSPI1_CS0
    /* Zeroed, non-cached ospi1_cs0 */
    __ospi1_cs0_zero_nocache$$ (NOLOAD) :
    {
        __ospi1_cs0_zero_nocache$$Base = .;
        /* section.ospi1_cs0.zero_nocache */
        *(.ospi1_cs0_nocache)
        . = ALIGN(32);
        __ospi1_cs0_zero_nocache$$Limit = .;
    }> OSPI1_CS0
    /* Non-initialized ospi1_cs0 */
    __ospi1_cs0_noinit$$ (NOLOAD) :
    {
        __ospi1_cs0_noinit$$Base = .;
        /* section.ospi1_cs0.noinit */
        *(.ospi1_cs0_noinit)
        __ospi1_cs0_noinit$$Limit = .;
    }> OSPI1_CS0
    /* Zeroed ospi1_cs0 */
    __ospi1_cs0_zero$$ (NOLOAD) :
    {
        __ospi1_cs0_zero$$Base = .;
        /* section.ospi1_cs0.zero */
        *(.ospi1_cs0)
        __ospi1_cs0_zero$$Limit = .;
    }> OSPI1_CS0
    .ospi1_cs0.endof ALIGN(.,512) :
    {
        __ddsc_OSPI1_CS0_END = .;

    }> OSPI1_CS0

    /***** ITCM memory section allocations ******/
    /* itcm initialized from flash */
    __itcm_from_flash$$ : ALIGN(8)
    {
        __itcm_from_flash$$Base = .;__itcm_from_flash$$Load = LOADADDR(__itcm_from_flash$$);
        /* section.itcm.from_flash */
        *(.itcm_from_flash)
        /* section.itcm.code_from_flash */
        *(.itcm_code_from_flash)
        /* ---- OMV-ITCM: 热函数白名单，按函数名抓取，上游源码零改动 ----
         * 必须放在本段（AT > FLASH，LMA 紧跟向量表），不能另开一段追加：
         * 后面 __flash_readonly$$ 含 *(.text*) catch-all，会先吃光这些输入段。
         * 也不能用 __itcm_from_ospi0_cs1$$（AT > OSPI0_CS1）——omv 的 XIP 改造
         * 让 FLASH_START 与 OSPI0_CS1 同址 0x90000000，LMA 会压住 __flash_vectors$$。
         */
        *(.text.mp_execute_bytecode)        /* Python 字节码解释主循环 */
        *(.text.imlib_draw_row*)            /* 图像行绘制（含 setup/teardown） */
        *(.text.imlib_draw_image*)          /* 图像缩放/绘制 */
        *(.text.arm_nn_mat_mult_nt_t_s8)    /* ↓ NN 卷积核：当前未被链接， */
        *(.text.arm_convolve_1x1_s8_fast)   /*   留着以便启用 TFLM 后自动生效 */
        *(.text.arm_convolve_s8)
        *(.text.arm_nn_depthwise_conv_nt_t_s8)
        *(.text.arm_depthwise_conv_s8_opt)
        . = ALIGN(8);
        __itcm_from_flash$$Limit = .;
    }> ITCM AT > FLASH
    /* Non-initialized itcm */
    __itcm_noinit$$ (NOLOAD) :
    {
        __itcm_noinit$$Base = .;
        /* section.itcm.noinit */
        *(.itcm_noinit)
        __itcm_noinit$$Limit = .;
    }> ITCM
    /* Zeroed itcm */
    __itcm_zero$$ (NOLOAD) :
    {
        __itcm_zero$$Base = .;
        /* section.itcm.zero */
        *(.itcm)
        __itcm_zero$$Limit = .;
    }> ITCM
    .itcm.endof ALIGN(.,512) :
    {
        __ddsc_ITCM_END = .;

    }> ITCM

    /***** DTCM memory section allocations ******/
    /* dtcm initialized from flash */
    __dtcm_from_flash$$ :
    {
        __dtcm_from_flash$$Base = .;__dtcm_from_flash$$Load = LOADADDR(__dtcm_from_flash$$);
        /* section.dtcm.from_flash */
        *(.dtcm_from_flash)
        /* section.dtcm.code_from_flash */
        *(.dtcm_code_from_flash)
        __dtcm_from_flash$$Limit = .;
    }> DTCM AT > FLASH
    /* Non-initialized dtcm */
    __dtcm_noinit$$ (NOLOAD) :
    {
        __dtcm_noinit$$Base = .;
        /* section.dtcm.noinit */
        *(.dtcm_noinit)
        __dtcm_noinit$$Limit = .;
    }> DTCM
    /* Zeroed dtcm */
    __dtcm_zero$$ (NOLOAD) :
    {
        __dtcm_zero$$Base = .;
        /* section.dtcm.zero */
        *(.dtcm)
        /* ★ 顺序很重要：栈必须排在 mp_state_ctx **之前**（低地址）。
         *
         * 栈向下生长。若 mp_state_ctx 在栈下方，一旦栈溢出，第一个被写坏的
         * 就是 MicroPython 的全局状态（GC 游标 / 调度状态 / VFS 挂载表），
         * 表现为「跑一段时间后莫名其妙出错」——而且没有任何保护会报警
         * （本工程 configCHECK_FOR_STACK_OVERFLOW = 0）。
         *
         * 与 Titan 一致：titan 的 fsp.ld 里 `*(.bss.dtcm*)`（含 vm_stack）
         * 排在 `*(.bss.mp_state_ctx)` 之前，栈溢出只会踩到 .dtcm/池子，
         * 不会破坏 VM 状态。 */
		*(.stack.mpy_thread)       /* OMV-DTCM：栈在此，向下生长 */
        *(.bss.mp_state_ctx)       /* OMV-DTCM：放在栈上方，避免被溢出写坏 */
		. = ALIGN(8);
        __dtcm_zero$$Limit = .;
    }> DTCM
    .dtcm.endof ALIGN(.,512) :
    {
        __ddsc_DTCM_END = .;

    }> DTCM

    /***** RAM memory section allocations ******/
    /* ram initialized from flash */
    __ram_from_flash$$ :
    {
        __ram_from_flash$$Base = .;__ram_from_flash$$Load = LOADADDR(__ram_from_flash$$);
        /* section.ram.from_flash */
        *(.ram_from_flash)
        /* section.ram.code_from_flash */
        *(.ram_code_from_flash)
        *(.data*)
        *(vtable)
        __ram_from_flash$$Limit = .;
    }> RAM AT > FLASH
    /* Non-initialized, non-cached ram */
    __ram_noinit_nocache$$ (NOLOAD) : ALIGN(32)
    {
        __ram_noinit_nocache$$Base = .;
        /* section.ram.noinit_nocache */
        *(.ram_noinit_nocache)
        __ram_noinit_nocache$$Limit = .;
    }> RAM
    /* Zeroed, non-cached ram */
    __ram_zero_nocache$$ (NOLOAD) :
    {
        __ram_zero_nocache$$Base = .;
        /* section.ram.zero_nocache */
        *(.ram_nocache)
        . = ALIGN(32);
        __ram_zero_nocache$$Limit = .;
    }> RAM
    /* Non-initialized ram */
    __ram_noinit$$ (NOLOAD) :
    {
        __ram_noinit$$Base = .;
        /* section.ram.noinit */
        *(.bss.g_heap)
        *(.bss.g_main_stack)
        *(.ram_noinit)
        *(.noinit)
        __ram_noinit$$Limit = .;
    }> RAM
    /* Zeroed ram */
    __ram_zero$$ (NOLOAD) :
    {
        __ram_zero$$Base = .;
        /* section.ram.zero */
        *(.ram)
        *(.bss*)
        __ram_zero$$Limit = .;
    }> RAM
    /* ---- OMV-ML: TFLM arena 专用静态池 --------------------------------
     * 为什么需要：arena 要一整块连续 SRAM（palm 需 866KB，hand 需 987KB），
     * 而 MicroPython 的 GC 没有压实（compaction）能力。实测 2026-10-22：
     * collect 后 SRAM 区空闲 1665KB，但最大连续段只有 433KB —— 几百个
     * 小对象就把整片空间切碎，且永远无法合并。arena 只能退回 SDRAM，
     * NPU 带宽降到约 40%，推理 379ms 而不是 38ms。
     *
     * 做法：在堆之前预留 1MB，让 arena 从这里切，彻底绕开 GC 碎片。
     * 位置必须在本段之后、__ram_thread_stack$$ 之前 —— _heap_start 等于
     * __ram_thread_stack$$Limit，所以 GC 的 SRAM 堆会自动缩小 1MB。
     *
     * 1MB 的依据（各模型 Vela 汇总表 Total SRAM used）：
     *   hand_landmarks 224 986.6KB | palm 192 865.3KB | yolov8n 192 815.8KB
     *   face_landmarks 192 540.0KB | blazeface 128 384.9KB | yolo_lc 257.8KB
     * 更大的模型会回落到 GC 路径（可能落 SDRAM，功能不受影响）。
     * -------------------------------------------------------------------- */
    .ml_arena_pool (NOLOAD) : ALIGN(32)
    {
        _ml_arena_pool_start = .;
        . = . + 0x00100000;   /* 1 MB，与 ra8_npu.c 的 OMV_ML_ARENA_POOL_SIZE 对应 */
        _ml_arena_pool_end = .;
    }> RAM
    /* Thread Stacks */
    __ram_thread_stack$$ (NOLOAD) : ALIGN(8)
    {
        __ram_thread_stack$$Base = .;
        KEEP(*(.stack?*))
        __ram_thread_stack$$Limit = .;
    }> RAM
    .ram.endof ALIGN(.,512) :
    {
        __ddsc_RAM_END = .;

    }> RAM
    .ram.flat_nsc ALIGN(.,512) :
    {
        __ddsc_RAM_NSC = .;

    }> RAM

    __flash_readonly$$ (READONLY) :
    {
        __flash_readonly$$Base = .;
        /* section.flash.readonly */
        *(.flash)
        /* section.flash.code */
        *(.flash_code)
        *(.text*)
        *(.rodata*)
        KEEP(*(.mcuboot_sce9_key))
        KEEP(*(.version))
        __flash_readonly$$Limit = .;
    }> FLASH
    __flash_ctor$$ (READONLY) :
    {

        *crtbegin.o(.ctors)
        *crtbegin?.o(.ctors)
        EXCLUDE_FILE (*crtend?.o *crtend.o) *(.ctors)
        *(SORT(.ctors.*))
        *(.ctors)
        *crtbegin.o(.dtors)
        *crtbegin?.o(.dtors)
        EXCLUDE_FILE (*crtend?.o *crtend.o) *(.dtors)
        *(SORT(.dtors.*))
        *(.dtors)

    }> FLASH
    __flash_preinit_array$$ (READONLY) :
    {
        __preinit_array_start = .;
        KEEP(*(.preinit_array))
        __preinit_array_end = .;
    }> FLASH
    __flash_.got$$ (READONLY) :
    {

        *(.got.plt)
        *(.got)

    }> FLASH
    __flash_init_array$$ (READONLY) :
    {
        __init_array_start = .;
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        __init_array_end = .;
    }> FLASH
    __flash_fini_array$$ (READONLY) :
    {
        __fini_array_start = .;
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        __fini_array_end = .;
    }> FLASH
    /* .ARM.extab sections contain exception unwinding information. */
    __flash_arm.extab$$ (READONLY) :
    {

        *(.ARM.extab*)
        *(.gnu.linkonce.armextab.*)

    }> FLASH
    /* .ARM.exidx sections contains index entries for section unwinding. */
    __flash_arm.exidx$$ (READONLY) :
    {
        __exidx_start = .;
        *(.ARM.exidx*)
        *(.gnu.linkonce.armexidx.*)
        __exidx_end = .;
    }> FLASH
    .flash.endof ALIGN(.,512) (READONLY) :
    {
        __ddsc_FLASH_END = .;

    }> FLASH
    .flash.flat_nsc ALIGN(.,512) (READONLY) :
    {
        __ddsc_FLASH_NSC = .;

    }> FLASH

    /***** OPTION_SETTING_OFS0 memory section allocations ******/
    .option_setting_ofs0.startof (READONLY) :
    {
        __ddsc_OPTION_SETTING_OFS0_START = .;

    }> OPTION_SETTING_OFS0
    /* Option Function Select Register 0 */
    __option_setting_ofs0_reg$$ (READONLY) :
    {
        __option_setting_ofs0_reg$$Base = .;
        KEEP(*(.option_setting_ofs0))
        __option_setting_ofs0_reg$$Limit = .;
    }> OPTION_SETTING_OFS0
    .option_setting_ofs0.endof (READONLY) :
    {
        __ddsc_OPTION_SETTING_OFS0_END = .;

    }> OPTION_SETTING_OFS0

    /***** OPTION_SETTING_OFS2 memory section allocations ******/
    .option_setting_ofs2.startof (READONLY) :
    {
        __ddsc_OPTION_SETTING_OFS2_START = .;

    }> OPTION_SETTING_OFS2
    /* Option Function Select Register 2 */
    __option_setting_ofs2_reg$$ (READONLY) :
    {
        __option_setting_ofs2_reg$$Base = .;
        KEEP(*(.option_setting_ofs2))
        __option_setting_ofs2_reg$$Limit = .;
    }> OPTION_SETTING_OFS2
    .option_setting_ofs2.endof (READONLY) :
    {
        __ddsc_OPTION_SETTING_OFS2_END = .;

    }> OPTION_SETTING_OFS2

    /***** OPTION_SETTING_SAS memory section allocations ******/
    .option_setting_sas.startof (READONLY) :
    {
        __ddsc_OPTION_SETTING_SAS_START = .;

    }> OPTION_SETTING_SAS
    /* Startup Area Setting Register */
    __option_setting_sas_reg$$ (READONLY) :
    {
        __option_setting_sas_reg$$Base = .;
        KEEP(*(.option_setting_sas))
        __option_setting_sas_reg$$Limit = .;
    }> OPTION_SETTING_SAS
    .option_setting_sas.endof (READONLY) :
    {
        __ddsc_OPTION_SETTING_SAS_END = .;

    }> OPTION_SETTING_SAS

    /***** OPTION_SETTING_OFS1 memory section allocations ******/
    .option_setting_ofs1.startof (READONLY) :
    {
        __ddsc_OPTION_SETTING_OFS1_START = .;

    }> OPTION_SETTING_OFS1
    /* Option Function Select Register 1 */
    __option_setting_ofs1_reg$$ (READONLY) :
    {
        __option_setting_ofs1_reg$$Base = .;
        KEEP(*(.option_setting_ofs1))
        __option_setting_ofs1_reg$$Limit = .;
    }> OPTION_SETTING_OFS1
    .option_setting_ofs1.endof (READONLY) :
    {
        __ddsc_OPTION_SETTING_OFS1_END = .;

    }> OPTION_SETTING_OFS1

    /***** OPTION_SETTING_OFS1_SEC memory section allocations ******/
    .option_setting_ofs1_sec.startof (READONLY) :
    {
        __ddsc_OPTION_SETTING_OFS1_SEC_START = .;

    }> OPTION_SETTING_OFS1_SEC
    /* Option Function Select Register 1 Secure */
    __option_setting_ofs1_sec_reg$$ (READONLY) :
    {
        __option_setting_ofs1_sec_reg$$Base = .;
        KEEP(*(.option_setting_ofs1_sec))
        __option_setting_ofs1_sec_reg$$Limit = .;
    }> OPTION_SETTING_OFS1_SEC
    .option_setting_ofs1_sec.endof (READONLY) :
    {
        __ddsc_OPTION_SETTING_OFS1_SEC_END = .;

    }> OPTION_SETTING_OFS1_SEC

    /***** OPTION_SETTING_OFS1_SEL memory section allocations ******/
    .option_setting_ofs1_sel.startof (READONLY) :
    {
        __ddsc_OPTION_SETTING_OFS1_SEL_START = .;

    }> OPTION_SETTING_OFS1_SEL
    /* OFS1 Security Attribution */
    __option_setting_ofs1_sel_reg$$ (READONLY) :
    {
        __option_setting_ofs1_sel_reg$$Base = .;
        KEEP(*(.option_setting_ofs1_sel))
        __option_setting_ofs1_sel_reg$$Limit = .;
    }> OPTION_SETTING_OFS1_SEL
    .option_setting_ofs1_sel.endof (READONLY) :
    {
        __ddsc_OPTION_SETTING_OFS1_SEL_END = .;

    }> OPTION_SETTING_OFS1_SEL

    /***** OPTION_SETTING_OFS3 memory section allocations ******/
    .option_setting_ofs3.startof (READONLY) :
    {
        __ddsc_OPTION_SETTING_OFS3_START = .;

    }> OPTION_SETTING_OFS3
    /* Option Function Select Register 3 */
    __option_setting_ofs3_reg$$ (READONLY) :
    {
        __option_setting_ofs3_reg$$Base = .;
        KEEP(*(.option_setting_ofs3))
        __option_setting_ofs3_reg$$Limit = .;
    }> OPTION_SETTING_OFS3
    .option_setting_ofs3.endof (READONLY) :
    {
        __ddsc_OPTION_SETTING_OFS3_END = .;

    }> OPTION_SETTING_OFS3

    /***** OPTION_SETTING_OFS3_SEC memory section allocations ******/
    .option_setting_ofs3_sec.startof (READONLY) :
    {
        __ddsc_OPTION_SETTING_OFS3_SEC_START = .;

    }> OPTION_SETTING_OFS3_SEC
    /* Option Function Select Register 3 Secure */
    __option_setting_ofs3_sec_reg$$ (READONLY) :
    {
        __option_setting_ofs3_sec_reg$$Base = .;
        KEEP(*(.option_setting_ofs3_sec))
        __option_setting_ofs3_sec_reg$$Limit = .;
    }> OPTION_SETTING_OFS3_SEC
    .option_setting_ofs3_sec.endof (READONLY) :
    {
        __ddsc_OPTION_SETTING_OFS3_SEC_END = .;

    }> OPTION_SETTING_OFS3_SEC

    /***** OPTION_SETTING_OFS3_SEL memory section allocations ******/
    .option_setting_ofs3_sel.startof (READONLY) :
    {
        __ddsc_OPTION_SETTING_OFS3_SEL_START = .;

    }> OPTION_SETTING_OFS3_SEL
    /* OFS3 Security Attribution */
    __option_setting_ofs3_sel_reg$$ (READONLY) :
    {
        __option_setting_ofs3_sel_reg$$Base = .;
        KEEP(*(.option_setting_ofs3_sel))
        __option_setting_ofs3_sel_reg$$Limit = .;
    }> OPTION_SETTING_OFS3_SEL
    .option_setting_ofs3_sel.endof (READONLY) :
    {
        __ddsc_OPTION_SETTING_OFS3_SEL_END = .;

    }> OPTION_SETTING_OFS3_SEL

    /***** OPTION_SETTING_BPS memory section allocations ******/
    .option_setting_bps.startof (READONLY) :
    {
        __ddsc_OPTION_SETTING_BPS_START = .;

    }> OPTION_SETTING_BPS
    /* Block Protect Setting Register */
    __option_setting_bps_reg$$ (READONLY) :
    {
        __option_setting_bps_reg$$Base = .;
        KEEP(*(.option_setting_bps))
        __option_setting_bps_reg$$Limit = .;
    }> OPTION_SETTING_BPS
    .option_setting_bps.endof (READONLY) :
    {
        __ddsc_OPTION_SETTING_BPS_END = .;

    }> OPTION_SETTING_BPS

    /***** OPTION_SETTING_BPS_SEC memory section allocations ******/
    .option_setting_bps_sec.startof (READONLY) :
    {
        __ddsc_OPTION_SETTING_BPS_SEC_START = .;

    }> OPTION_SETTING_BPS_SEC
    /* Block Protect Setting Register Secure */
    __option_setting_bps_sec_reg$$ (READONLY) :
    {
        __option_setting_bps_sec_reg$$Base = .;
        KEEP(*(.option_setting_bps_sec))
        __option_setting_bps_sec_reg$$Limit = .;
    }> OPTION_SETTING_BPS_SEC
    .option_setting_bps_sec.endof (READONLY) :
    {
        __ddsc_OPTION_SETTING_BPS_SEC_END = .;

    }> OPTION_SETTING_BPS_SEC

    /***** OPTION_SETTING_OTP_FSBLCTRL0 memory section allocations ******/
    .option_setting_otp_fsblctrl0.startof (READONLY) :
    {
        __ddsc_OPTION_SETTING_OTP_FSBLCTRL0_START = .;

    }> OPTION_SETTING_OTP_FSBLCTRL0
    /* FSBL Control Register 0 */
    __option_setting_otp_fsblctrl0_reg$$ (READONLY) :
    {
        __option_setting_otp_fsblctrl0_reg$$Base = .;
        KEEP(*(.option_setting_otp_fsblctrl0))
        __option_setting_otp_fsblctrl0_reg$$Limit = .;
    }> OPTION_SETTING_OTP_FSBLCTRL0
    .option_setting_otp_fsblctrl0.endof (READONLY) :
    {
        __ddsc_OPTION_SETTING_OTP_FSBLCTRL0_END = .;

    }> OPTION_SETTING_OTP_FSBLCTRL0

    /***** OPTION_SETTING_OTP_FSBLCTRL1 memory section allocations ******/
    .option_setting_otp_fsblctrl1.startof (READONLY) :
    {
        __ddsc_OPTION_SETTING_OTP_FSBLCTRL1_START = .;

    }> OPTION_SETTING_OTP_FSBLCTRL1
    /* FSBL Control Register 1 */
    __option_setting_otp_fsblctrl1_reg$$ (READONLY) :
    {
        __option_setting_otp_fsblctrl1_reg$$Base = .;
        KEEP(*(.option_setting_otp_fsblctrl1))
        __option_setting_otp_fsblctrl1_reg$$Limit = .;
    }> OPTION_SETTING_OTP_FSBLCTRL1
    .option_setting_otp_fsblctrl1.endof (READONLY) :
    {
        __ddsc_OPTION_SETTING_OTP_FSBLCTRL1_END = .;

    }> OPTION_SETTING_OTP_FSBLCTRL1

    /***** OPTION_SETTING_OTP_FSBLCTRL2 memory section allocations ******/
    .option_setting_otp_fsblctrl2.startof (READONLY) :
    {
        __ddsc_OPTION_SETTING_OTP_FSBLCTRL2_START = .;

    }> OPTION_SETTING_OTP_FSBLCTRL2
    /* FSBL Control Register 2 */
    __option_setting_otp_fsblctrl2_reg$$ (READONLY) :
    {
        __option_setting_otp_fsblctrl2_reg$$Base = .;
        KEEP(*(.option_setting_otp_fsblctrl2))
        __option_setting_otp_fsblctrl2_reg$$Limit = .;
    }> OPTION_SETTING_OTP_FSBLCTRL2
    .option_setting_otp_fsblctrl2.endof (READONLY) :
    {
        __ddsc_OPTION_SETTING_OTP_FSBLCTRL2_END = .;

    }> OPTION_SETTING_OTP_FSBLCTRL2

    /***** OPTION_SETTING_OTP_SAMR memory section allocations ******/
    .option_setting_otp_samr.startof (READONLY) :
    {
        __ddsc_OPTION_SETTING_OTP_SAMR_START = .;

    }> OPTION_SETTING_OTP_SAMR
    /* Start Address of Measurement Report */
    __option_setting_otp_samr_reg$$ (READONLY) :
    {
        __option_setting_otp_samr_reg$$Base = .;
        KEEP(*(.option_setting_otp_samr))
        __option_setting_otp_samr_reg$$Limit = .;
    }> OPTION_SETTING_OTP_SAMR
    .option_setting_otp_samr.endof (READONLY) :
    {
        __ddsc_OPTION_SETTING_OTP_SAMR_END = .;

    }> OPTION_SETTING_OTP_SAMR

    /***** OPTION_SETTING_OTP_SACC00 memory section allocations ******/
    .option_setting_otp_sacc00.startof (READONLY) :
    {
        __ddsc_OPTION_SETTING_OTP_SACC00_START = .;

    }> OPTION_SETTING_OTP_SACC00
    /* Start Address of Code Certificate 00 */
    __option_setting_otp_sacc00_reg$$ (READONLY) :
    {
        __option_setting_otp_sacc00_reg$$Base = .;
        KEEP(*(.option_setting_otp_sacc00))
        __option_setting_otp_sacc00_reg$$Limit = .;
    }> OPTION_SETTING_OTP_SACC00
    .option_setting_otp_sacc00.endof (READONLY) :
    {
        __ddsc_OPTION_SETTING_OTP_SACC00_END = .;

    }> OPTION_SETTING_OTP_SACC00

    /***** OPTION_SETTING_OTP_SACC10 memory section allocations ******/
    .option_setting_otp_sacc10.startof (READONLY) :
    {
        __ddsc_OPTION_SETTING_OTP_SACC10_START = .;

    }> OPTION_SETTING_OTP_SACC10
    /* Start Address of Code Certificate 10 */
    __option_setting_otp_sacc10_reg$$ (READONLY) :
    {
        __option_setting_otp_sacc10_reg$$Base = .;
        KEEP(*(.option_setting_otp_sacc10))
        __option_setting_otp_sacc10_reg$$Limit = .;
    }> OPTION_SETTING_OTP_SACC10
    .option_setting_otp_sacc10.endof (READONLY) :
    {
        __ddsc_OPTION_SETTING_OTP_SACC10_END = .;

    }> OPTION_SETTING_OTP_SACC10

    /***** OPTION_SETTING_OTP_SACC01 memory section allocations ******/
    .option_setting_otp_sacc01.startof (READONLY) :
    {
        __ddsc_OPTION_SETTING_OTP_SACC01_START = .;

    }> OPTION_SETTING_OTP_SACC01
    /* Start Address of Code Certificate 01 */
    __option_setting_otp_sacc01_reg$$ (READONLY) :
    {
        __option_setting_otp_sacc01_reg$$Base = .;
        KEEP(*(.option_setting_otp_sacc01))
        __option_setting_otp_sacc01_reg$$Limit = .;
    }> OPTION_SETTING_OTP_SACC01
    .option_setting_otp_sacc01.endof (READONLY) :
    {
        __ddsc_OPTION_SETTING_OTP_SACC01_END = .;

    }> OPTION_SETTING_OTP_SACC01

    /***** OPTION_SETTING_OTP_SACC11 memory section allocations ******/
    .option_setting_otp_sacc11.startof (READONLY) :
    {
        __ddsc_OPTION_SETTING_OTP_SACC11_START = .;

    }> OPTION_SETTING_OTP_SACC11
    /* Start Address of Code Certificate 11 */
    __option_setting_otp_sacc11_reg$$ (READONLY) :
    {
        __option_setting_otp_sacc11_reg$$Base = .;
        KEEP(*(.option_setting_otp_sacc11))
        __option_setting_otp_sacc11_reg$$Limit = .;
    }> OPTION_SETTING_OTP_SACC11
    .option_setting_otp_sacc11.endof (READONLY) :
    {
        __ddsc_OPTION_SETTING_OTP_SACC11_END = .;

    }> OPTION_SETTING_OTP_SACC11

    /***** OPTION_SETTING_OTP_SACC02 memory section allocations ******/
    .option_setting_otp_sacc02.startof (READONLY) :
    {
        __ddsc_OPTION_SETTING_OTP_SACC02_START = .;

    }> OPTION_SETTING_OTP_SACC02
    /* Start Address of Code Certificate 02 */
    __option_setting_otp_sacc02_reg$$ (READONLY) :
    {
        __option_setting_otp_sacc02_reg$$Base = .;
        KEEP(*(.option_setting_otp_sacc02))
        __option_setting_otp_sacc02_reg$$Limit = .;
    }> OPTION_SETTING_OTP_SACC02
    .option_setting_otp_sacc02.endof (READONLY) :
    {
        __ddsc_OPTION_SETTING_OTP_SACC02_END = .;

    }> OPTION_SETTING_OTP_SACC02

    /***** OPTION_SETTING_OTP_SACC12 memory section allocations ******/
    .option_setting_otp_sacc12.startof (READONLY) :
    {
        __ddsc_OPTION_SETTING_OTP_SACC12_START = .;

    }> OPTION_SETTING_OTP_SACC12
    /* Start Address of Code Certificate 12 */
    __option_setting_otp_sacc12_reg$$ (READONLY) :
    {
        __option_setting_otp_sacc12_reg$$Base = .;
        KEEP(*(.option_setting_otp_sacc12))
        __option_setting_otp_sacc12_reg$$Limit = .;
    }> OPTION_SETTING_OTP_SACC12
    .option_setting_otp_sacc12.endof (READONLY) :
    {
        __ddsc_OPTION_SETTING_OTP_SACC12_END = .;

    }> OPTION_SETTING_OTP_SACC12

    /***** OPTION_SETTING_OTP_SACC03 memory section allocations ******/
    .option_setting_otp_sacc03.startof (READONLY) :
    {
        __ddsc_OPTION_SETTING_OTP_SACC03_START = .;

    }> OPTION_SETTING_OTP_SACC03
    /* Start Address of Code Certificate 03 */
    __option_setting_otp_sacc03_reg$$ (READONLY) :
    {
        __option_setting_otp_sacc03_reg$$Base = .;
        KEEP(*(.option_setting_otp_sacc03))
        __option_setting_otp_sacc03_reg$$Limit = .;
    }> OPTION_SETTING_OTP_SACC03
    .option_setting_otp_sacc03.endof (READONLY) :
    {
        __ddsc_OPTION_SETTING_OTP_SACC03_END = .;

    }> OPTION_SETTING_OTP_SACC03

    /***** OPTION_SETTING_OTP_SACC13 memory section allocations ******/
    .option_setting_otp_sacc13.startof (READONLY) :
    {
        __ddsc_OPTION_SETTING_OTP_SACC13_START = .;

    }> OPTION_SETTING_OTP_SACC13
    /* Start Address of Code Certificate 13 */
    __option_setting_otp_sacc13_reg$$ (READONLY) :
    {
        __option_setting_otp_sacc13_reg$$Base = .;
        KEEP(*(.option_setting_otp_sacc13))
        __option_setting_otp_sacc13_reg$$Limit = .;
    }> OPTION_SETTING_OTP_SACC13
    .option_setting_otp_sacc13.endof (READONLY) :
    {
        __ddsc_OPTION_SETTING_OTP_SACC13_END = .;

    }> OPTION_SETTING_OTP_SACC13

    /***** OPTION_SETTING_OTP_PBPS_SEC memory section allocations ******/
    .option_setting_otp_pbps_sec.startof (READONLY) :
    {
        __ddsc_OPTION_SETTING_OTP_PBPS_SEC_START = .;

    }> OPTION_SETTING_OTP_PBPS_SEC
    /* Permanent Block Protect Setting Register Secure */
    __option_setting_otp_pbps_sec_reg$$ (READONLY) :
    {
        __option_setting_otp_pbps_sec_reg$$Base = .;
        KEEP(*(.option_setting_otp_pbps_sec))
        __option_setting_otp_pbps_sec_reg$$Limit = .;
    }> OPTION_SETTING_OTP_PBPS_SEC
    .option_setting_otp_pbps_sec.endof (READONLY) :
    {
        __ddsc_OPTION_SETTING_OTP_PBPS_SEC_END = .;

    }> OPTION_SETTING_OTP_PBPS_SEC

    /***** OPTION_SETTING_OTP_PBPS memory section allocations ******/
    .option_setting_otp_pbps.startof (READONLY) :
    {
        __ddsc_OPTION_SETTING_OTP_PBPS_START = .;

    }> OPTION_SETTING_OTP_PBPS
    /* Permanent Block Protect Setting Register */
    __option_setting_otp_pbps_reg$$ (READONLY) :
    {
        __option_setting_otp_pbps_reg$$Base = .;
        KEEP(*(.option_setting_otp_pbps))
        __option_setting_otp_pbps_reg$$Limit = .;
    }> OPTION_SETTING_OTP_PBPS
    .option_setting_otp_pbps.endof (READONLY) :
    {
        __ddsc_OPTION_SETTING_OTP_PBPS_END = .;

    }> OPTION_SETTING_OTP_PBPS

    /***** OPTION_SETTING_OTP_ZHUK memory section allocations ******/
    .option_setting_otp_zhuk.startof (READONLY) :
    {
        __ddsc_OPTION_SETTING_OTP_ZHUK_START = .;

    }> OPTION_SETTING_OTP_ZHUK
    /* Zerorization HUK Register */
    __option_setting_otp_zhuk_reg$$ (READONLY) :
    {
        __option_setting_otp_zhuk_reg$$Base = .;
        KEEP(*(.option_setting_otp_zhuk))
        __option_setting_otp_zhuk_reg$$Limit = .;
    }> OPTION_SETTING_OTP_ZHUK
    .option_setting_otp_zhuk.endof (READONLY) :
    {
        __ddsc_OPTION_SETTING_OTP_ZHUK_END = .;

    }> OPTION_SETTING_OTP_ZHUK

}

/* ---- OMV-ITCM: ECC 对齐与容量断言 ----
 * ITCM 有 ECC，FSP 的拷贝以双字为单位（copy_64）。虽然当前 system.c 走的是
 * memcpy，仍按 Titan 的做法保证两端 8 字节对齐，避免依赖巧合。
 * 加/减 ITCM 函数后，若这里报错就说明需要调整白名单。
 */
ASSERT((__itcm_from_flash$$Base & 7) == 0, "OMV-ITCM: ITCM base not 8-byte aligned")
ASSERT((__itcm_from_flash$$Limit & 7) == 0, "OMV-ITCM: ITCM end not 8-byte aligned (ECC)")
ASSERT(__itcm_from_flash$$Limit <= ORIGIN(ITCM) + LENGTH(ITCM), "OMV-ITCM: ITCM overflow (>128KB)")

/* ---- OMV-VISION 回归断言 ------------------------------------------------
 * 2026-10-12 的故障：往 __sdram_from_flash$$ 塞 imlib 后，该段 VMA 落在
 * SDRAM_START，把 VIN 采集缓冲顶进 GC 第二堆（0x681C2000 起），运行期总线错误。
 * 这两条断言就是拦那个 bug 的：VIN 缓冲必须停在 GC 堆起点之前，
 * imlib 必须落在专用的高位区。 */
ASSERT(__sdram_noinit_nocache$$Limit <= 0x681C2000,
       "OMV-VISION: VIN buffers overlap the hardcoded GC heap at 0x681C2000")
ASSERT(__sdram_from_flash$$Base == 0x68800000,
       "OMV-VISION: imlib code must sit at 0x68800000")
ASSERT(__sdram_from_flash$$Limit <= ORIGIN(OMV_CODE) + LENGTH(OMV_CODE),
       "OMV-VISION: imlib code overflows the 2 MB OMV_CODE region")

/* ---- OMV-UMA 边界 -------------------------------------------------------
 * UMA 主池不再是 MEMORY 区，只是 fsp.ld 里的 _uma_pool_start/_end 两个符号，
 * 链接器不会替我们挡越界。这条断言把 OMV_CODE 上沿和 UMA 起点绑死：
 * 以后有人调大 OMV_CODE 的 LENGTH，或挪动 _uma_pool_start，都会在这里失败。 */
ASSERT(ORIGIN(OMV_CODE) + LENGTH(OMV_CODE) <= _uma_pool_start,
       "OMV-VISION: OMV_CODE overruns the UMA pool at _uma_pool_start")
ASSERT(_uma_pool_start < _uma_pool_end,
       "OMV-VISION: _uma_pool_end must be above _uma_pool_start")


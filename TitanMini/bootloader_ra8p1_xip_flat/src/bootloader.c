/*
* Copyright (c) 2020 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*
 * 外部 flash 上的应用启动入口 —— mcuboot 的替代品。
 *
 * 不依赖 mcuboot 的任何头文件或函数：不读镜像头、不验签、不调 boot_go()，
 * 也不调 rm_mcuboot_cleanup()。目标地址是编译期算好的常量，直接跳。
 *
 * 收尾步骤参考 RM_MCUBOOT_PORT_BootApp()（ra/fsp/src/rm_mcuboot_port/
 * rm_mcuboot_port.c）的次序：解除栈指针下限 -> 设 VTOR -> naked 换栈跳转。
 * 不关 UART / OSPI，理由见 jump_disable_stack_monitor() 上方注释。
 *
 * 单核：只启动当前核。
 */

#include <stdio.h>

#include "hal_data.h"

/* ---------------------------------------------------------------------------
 * 目标地址（全部编译期算好）
 * ------------------------------------------------------------------------- */

/* 槽基址：外部 OSPI0_CS1 flash 上的 image0 secondary。
 * 对应 bsp_linker_info.h 的 BSP_PARTITION___BL_0_S_H_START。 */
#define BOOT_SLOT_BASE        (0x90000000UL)

/* 槽头部预留区：mcuboot 镜像头占用的空间。
 * 应用链接脚本同样从这个偏移起算 ——
 *   secondary_cpu0_ra8p1/script/memory_regions_sec.ld: FLASH_START = 0x90000200
 * 所以无论镜像是否带签名，向量表都在这里。 */
#define BOOT_HEADER_SIZE      (0x000UL)

/* 向量表地址 = 槽基址 + 头长度 = 0x90000200。 */
#define BOOT_VECTOR_TABLE     (BOOT_SLOT_BASE + BOOT_HEADER_SIZE)

/* 目标镜像的内部 RAM 范围，取自其链接脚本
 *   secondary_cpu0_ra8p1/script/memory_regions_sec.ld:
 *     RAM_START = 0x22004800, RAM_LENGTH = 0x180000  ->  末端 0x22184800
 * 栈顶必须落在这段里，否则第一次压栈就会打到无效内存。 */

#define BOOT_APP_RAM_START    (0x22000000UL)
#define BOOT_APP_RAM_END      (0x221D4000UL)

/* 向量表头两项。 */
typedef struct
{
    uint32_t sp;                       /* [0] 初始 MSP */
    uint32_t pc;                       /* [1] Reset_Handler（带 Thumb 位） */
} jump_vector_t;

/* ---------------------------------------------------------------------------
 * 步骤 1：向量表地址必须在 flash 内且 4 字节对齐 —— 表项都是 4 字节宽。
 * ------------------------------------------------------------------------- */
static bool jump_check_vector_address (uint32_t vector_table)
{
    if (0U != (vector_table & 0x3U))
    {
        printf("boot: vector table 0x%08lx is not 4-byte aligned\n",
               (unsigned long) vector_table);

        return false;
    }

    return true;
}

/* ---------------------------------------------------------------------------
 * 步骤 2：从向量表读初始 SP 和入口地址。
 * ------------------------------------------------------------------------- */
static void jump_load_vectors (uint32_t vector_table, jump_vector_t * p_vectors)
{
    const uint32_t * p_table = (const uint32_t *) vector_table;

    p_vectors->sp = p_table[0];
    p_vectors->pc = p_table[1];
}

/* ---------------------------------------------------------------------------
 * 步骤 3：校验读到的表项。
 *
 * 没有签名可验，这三项是唯一能挡住"槽里没东西/装错了"的防线：
 *   - 空槽是全 0xFF，SP = 0xFFFFFFFF 会在 8 字节对齐这一关被拦下
 *   - Thumb 位是架构强制的，Reset_Handler 入口最低位必为 1
 *   - SP 必须落在目标镜像的 RAM 范围内，否则跳过去必硬 fault
 * 失败时打印实际读到的值。
 * ------------------------------------------------------------------------- */
static bool jump_validate_vectors (const jump_vector_t * p_vectors)
{
    if (0U == (p_vectors->pc & 0x1U))
    {
        printf("boot: bad entry 0x%08lx (thumb bit not set)\n",
               (unsigned long) p_vectors->pc);

        return false;
    }

    if (0U != (p_vectors->sp & 0x7U))
    {
        printf("boot: bad stack 0x%08lx (not 8-byte aligned)\n",
               (unsigned long) p_vectors->sp);

        return false;
    }

    if ((p_vectors->sp < BOOT_APP_RAM_START) || (p_vectors->sp > BOOT_APP_RAM_END))
    {
        printf("boot: bad stack 0x%08lx (outside app RAM 0x%08lx..0x%08lx)\n",
               (unsigned long) p_vectors->sp,
               (unsigned long) BOOT_APP_RAM_START,
               (unsigned long) BOOT_APP_RAM_END);

        return false;
    }

    return true;
}

/* ---------------------------------------------------------------------------
 * 步骤 4：解除 bootloader 设下的栈指针下限。
 *
 * FSP 的 SystemInit() 会用 __set_MSPLIM(BSP_PRV_STACK_LIMIT) 给自己的栈设下限；
 * 应用换了 SP 之后如果这个下限还在，第一次压栈就越界。
 * 与 RM_MCUBOOT_PORT_BootApp() 的做法一致。
 *
 * UART / OSPI 都不在这里关：
 *   - R_OSPI_B_Close() 会走到 R_BSP_MODULE_STOP(FSP_IP_OSPI) 停掉 OSPI 时钟，
 *     而目标镜像正是 XIP 在 0x90000000，关了当场取不到指令
 *   - UART 由应用自己的 Open() 重新初始化，bootloader 的 p_callback 是 NULL，
 *     没开过中断，留着不动没有副作用
 * ------------------------------------------------------------------------- */
static void jump_disable_stack_monitor (void)
{
#if BSP_FEATURE_TZ_HAS_TRUSTZONE
    __set_MSPLIM(0);
#elif BSP_FEATURE_BSP_HAS_SP_MON
    R_MPU_SPMON->SP[0].CTL = 0;
#endif
}

/* ---------------------------------------------------------------------------
 * 步骤 5：把向量表切到目标镜像。
 * 应用侧的 SystemInit() 之后还会再设一次，这里只是让跳过去的瞬间就位。
 * ------------------------------------------------------------------------- */
static void jump_install_vector_table (uint32_t vector_table)
{
    SCB->VTOR = vector_table;
    __DSB();
    __ISB();
}

/* ---------------------------------------------------------------------------
 * 步骤 6：换栈并跳转。
 *
 * naked 函数，编译器不生成序言/尾声 —— 栈换掉之后没有旧栈帧可以返回。
 * R0/R1 是 AAPCS 的前两个整型参数，对应 pc / sp。
 * ------------------------------------------------------------------------- */
static void jump_enter_application (uint32_t pc, uint32_t sp) __attribute__((naked, no_instrument_function));

static void jump_enter_application (uint32_t pc __attribute__((unused)), uint32_t sp __attribute__((unused)))
{
    __asm volatile (
        "MSR     MSP, R1                         \n"
        "DSB                                     \n"
        "ISB                                     \n"
        "BX      R0                              \n"
        );
}

/* ---------------------------------------------------------------------------
 * 启动外部 flash 上的镜像。无参数。
 *
 * 调用方需自行保证串口已打开且 OSPI 已 Open（目标镜像是 XIP 执行的）。
 *
 * @retval true   已跳转（正常不会返回）
 * @retval false  校验没过，未跳转
 * ------------------------------------------------------------------------- */
bool JumpToNewFirmware (void)
{
    jump_vector_t vectors;

    if (!jump_check_vector_address(BOOT_VECTOR_TABLE))
    {
        return false;
    }

    jump_load_vectors(BOOT_VECTOR_TABLE, &vectors);

    if (!jump_validate_vectors(&vectors))
    {
        return false;
    }

    printf("boot: slot=0x%08lx vector=0x%08lx entry=0x%08lx stack=0x%08lx\n",
           (unsigned long) BOOT_SLOT_BASE, (unsigned long) BOOT_VECTOR_TABLE,
           (unsigned long) vectors.pc, (unsigned long) vectors.sp);

    jump_disable_stack_monitor();
    jump_install_vector_table(BOOT_VECTOR_TABLE);

     jump_enter_application(vectors.pc, vectors.sp);

    /* 不会走到这里。 */
    return true;
}

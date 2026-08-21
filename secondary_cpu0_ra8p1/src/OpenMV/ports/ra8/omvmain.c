/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (C) 2025 OpenMV, LLC.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software and to permit persons to whom the Software is
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
 * RA8P1 OpenMV + MicroPython main entry (FreeRTOS task).
 */
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#include "py/builtin.h"
#include "py/compile.h"
#include "py/runtime.h"
#include "py/repl.h"
#include "py/gc.h"
#include "py/mperrno.h"
#include "py/stackctrl.h"
#include "shared/runtime/pyexec.h"
#include "shared/runtime/softtimer.h"
#include "shared/readline/readline.h"
#include "gccollect.h"
#include "extmod/vfs.h"
#include "extmod/vfs_fat.h"
#include "FreeRTOS.h"
#include "task.h"

#include "omv_boardconfig.h"
#include "omv_gpio.h"
#include "omv_i2c.h"
#include "omv_csi.h"
#include "omv_protocol.h"
#include "mp_utils.h"
#include "fb_alloc.h"
#include "dma_alloc.h"
#include "file_utils.h"

#include "py_image.h"
#include "py_fir.h"
#include "py_tv.h"
#include "py_imu.h"

#include "bsp_api.h"
#include "tusb.h"


#include "hal_data.h"
#include "drivers/fsp_qspi_flash.h"

int errno;

void NORETURN __fatal_error(const char *msg) {
    for (volatile uint32_t delay = 0;; delay++) {
        if (delay > 10000000) {
            __WFI();
        }
    }
}

void nlr_jump_fail(void *val) {
    printf("FATAL: uncaught exception %p\n", val);
    __fatal_error("");
}

#ifndef NDEBUG
void __attribute__((weak)) __assert_func(const char *file, int line, const char *func, const char *expr) {
    (void) func;
    printf("Assertion '%s' failed, at file %s:%d\n", expr, file, line);
    __fatal_error("");
}
#endif

#ifdef STACK_PROTECTOR
uint32_t __stack_chk_guard = 0xDEADBEEF;

void NORETURN __stack_chk_fail(void) {
    __fatal_error("stack check failed");
}
#endif

static fs_user_mount_t qspi_flash_fs;

static int qspi_flash_init_fs(void) {
    QSPI_Flash_Init();
    fs_user_mount_t *vfs_fat = &qspi_flash_fs;
    vfs_fat->blockdev.flags = 0;
    qspi_flash_init_vfs(vfs_fat);

    FRESULT res = f_mount(&vfs_fat->fatfs);
    if (res == FR_NO_FILESYSTEM) {
        static uint8_t working_buf[FF_MAX_SS];
        res = f_mkfs(&vfs_fat->fatfs, FM_FAT, 0, working_buf, sizeof(working_buf));
        if (res == FR_OK) {
            res = f_mount(&vfs_fat->fatfs);
        }
    }
    if (res != FR_OK) return -1;

    mp_vfs_mount_t *vfs = m_new_obj_maybe(mp_vfs_mount_t);
    if (vfs == NULL) return -1;
    vfs->str = "/flash";
    vfs->len = 6;
    vfs->obj = MP_OBJ_FROM_PTR(vfs_fat);
    vfs->next = NULL;
    MP_STATE_VM(vfs_mount_table) = vfs;
    return 0;
}

// extern void mipi_csi_test(void);


/* OpenMV + MicroPython main entry — called from FreeRTOS task */
void omv_main(void *pvParameters) {
    FSP_PARAMETER_NOT_USED(pvParameters);
    bool first_soft_reset = true;

    /* One-time hardware init */
    board_init();
    mp_uart_repl_init();

#if MICROPY_HW_ENABLE_USBDEV
    if (!tusb_inited()) {
        tusb_init();
    }
#endif
    
// mipi test
//    mipi_csi_test();
//    while(1){
//
//    }
soft_reset:

    /* Initialize stack control for this FreeRTOS task */
    mp_cstack_init_with_sp_here(0x8000);

    /* GC init */
    gc_init(MICROPY_HEAP_START, MICROPY_HEAP_END);

    /* OpenMV low-level subsystems */
    imlib_init();
    readline_init0();
    fb_alloc_init0();
    framebuffer_init0();

#if MICROPY_PY_CSI
    omv_csi_init0();
#endif
#if OMV_DMA_ALLOC
    dma_alloc_init0();
#endif
//#ifdef IMLIB_ENABLE_IMAGE_FILE_IO
//    file_buffer_init0();
//#endif

    /* MicroPython init */
    mp_init();

    /* Init OpenMV protocol */
    omv_protocol_init_default();

    /* Init camera if first boot */
#if MICROPY_PY_CSI
    if (first_soft_reset) {
        int ret = omv_csi_init();
        if (ret != 0 && ret != OMV_CSI_ERROR_ISC_UNDETECTED) {
            __fatal_error("Failed to init the CSI");
        }
    }
#endif

    if (pyexec_mode_kind == PYEXEC_MODE_RAW_REPL) {
        mp_hal_set_interrupt_char(CHAR_CTRL_C);
    }

    while (!omv_protocol_exec_script()) {
        nlr_buf_t nlr;

        if (nlr_push(&nlr) == 0) {
            mp_hal_set_interrupt_char(CHAR_CTRL_C);

            if (pyexec_mode_kind == PYEXEC_MODE_RAW_REPL) {
                if (pyexec_raw_repl() != 0) {
                    break;
                }
            } else {
                if (pyexec_friendly_repl() != 0) {
                    break;
                }
            }
            nlr_pop();
        }
    }

    // soft reset
    mp_hal_set_interrupt_char(-1);
    mp_printf(MP_PYTHON_PRINTER, "MPY: soft reboot\n");
#if MICROPY_PY_CSI
    omv_csi_abort_all();
#endif
    imlib_deinit();
    soft_timer_deinit();
#if MICROPY_HW_ENABLE_USBDEV && MICROPY_HW_ENABLE_USB_RUNTIME_DEVICE
    mp_usbd_deinit();
#endif
    gc_sweep_all();
    mp_deinit();
    first_soft_reset = false;
    goto soft_reset;
}

mp_lexer_t *mp_lexer_new_from_file(qstr filename) {
    mp_raise_OSError(MP_ENOENT);
}

void abort(void) {
    __fatal_error("abort");
}


#if MICROPY_HW_ENABLE_USBDEV
void usbfs_interrupt_handler(void) {
    IRQn_Type irq = R_FSP_CurrentIrqGet();
    R_BSP_IrqStatusClear(irq);

    #if CFG_TUSB_RHPORT0_MODE & OPT_MODE_HOST
    tuh_int_handler(0);
    #endif

    #if CFG_TUSB_RHPORT0_MODE & OPT_MODE_DEVICE
    tud_int_handler(0);
    #endif
}

void usbfs_resume_handler(void) {
    IRQn_Type irq = R_FSP_CurrentIrqGet();
    R_BSP_IrqStatusClear(irq);

    #if CFG_TUSB_RHPORT0_MODE & OPT_MODE_HOST
    tuh_int_handler(0);
    #endif

    #if CFG_TUSB_RHPORT0_MODE & OPT_MODE_DEVICE
    tud_int_handler(0);
    #endif
}

void usbfs_d0fifo_handler(void) {
    IRQn_Type irq = R_FSP_CurrentIrqGet();
    R_BSP_IrqStatusClear(irq);

    #if CFG_TUSB_RHPORT0_MODE & OPT_MODE_HOST
    tuh_int_handler(0);
    #endif

    #if CFG_TUSB_RHPORT0_MODE & OPT_MODE_DEVICE
    tud_int_handler(0);
    #endif
}

void usbfs_d1fifo_handler(void) {
    IRQn_Type irq = R_FSP_CurrentIrqGet();
    R_BSP_IrqStatusClear(irq);

    #if CFG_TUSB_RHPORT0_MODE & OPT_MODE_HOST
    tuh_int_handler(0);
    #endif

    #if CFG_TUSB_RHPORT0_MODE & OPT_MODE_DEVICE
    tud_int_handler(0);
    #endif
}

void usbhs_interrupt_handler(void) {
    IRQn_Type irq = R_FSP_CurrentIrqGet();
    R_BSP_IrqStatusClear(irq);

    #if CFG_TUSB_RHPORT1_MODE & OPT_MODE_HOST
    tuh_int_handler(1);
    #endif

    #if CFG_TUSB_RHPORT1_MODE & OPT_MODE_DEVICE
    tud_int_handler(1);
    #endif
}

void usbhs_d0fifo_handler(void) {
    IRQn_Type irq = R_FSP_CurrentIrqGet();
    R_BSP_IrqStatusClear(irq);

    #if CFG_TUSB_RHPORT1_MODE & OPT_MODE_HOST
    tuh_int_handler(1);
    #endif

    #if CFG_TUSB_RHPORT1_MODE & OPT_MODE_DEVICE
    tud_int_handler(1);
    #endif
}

void usbhs_d1fifo_handler(void) {
    IRQn_Type irq = R_FSP_CurrentIrqGet();
    R_BSP_IrqStatusClear(irq);

    #if CFG_TUSB_RHPORT1_MODE & OPT_MODE_HOST
    tuh_int_handler(1);
    #endif

    #if CFG_TUSB_RHPORT1_MODE & OPT_MODE_DEVICE
    tud_int_handler(1);
    #endif
}
#endif

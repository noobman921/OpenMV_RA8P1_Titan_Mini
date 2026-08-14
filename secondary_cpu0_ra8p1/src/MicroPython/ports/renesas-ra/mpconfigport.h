#ifndef MICROPYTHON_PORTS_RENESAS_RA_MOCONFIGPORT_H_
#define MICROPYTHON_PORTS_RENESAS_RA_MOCONFIGPORT_H_
#include <stdint.h>
//#include "hal_data.h"
#include "mpy_thread.h"
#include "mpconfigboard.h"

// options to control how MicroPython is built

// Use the minimal starting configuration (disables all optional features).
//#define MICROPY_CONFIG_ROM_LEVEL (MICROPY_CONFIG_ROM_LEVEL_MINIMUM)
#define MICROPY_CONFIG_ROM_LEVEL (MICROPY_CONFIG_ROM_LEVEL_EXTRA_FEATURES)
// You can disable the built-in MicroPython compiler by setting the following
// config option to 0.  If you do this then you won't get a REPL prompt, but you
// will still be able to execute pre-compiled scripts, compiled with mpy-cross.
#define MICROPY_ENABLE_COMPILER     (1)

// 核心功能
#define MICROPY_ENABLE_GC                 (1) //垃圾回收
#define MICROPY_HELPER_REPL               (1) //REPL
#define MICROPY_GCREGS_SETJMP             (1)

//#define MICROPY_QSTR_EXTRA_POOL           mp_qstr_frozen_const_pool
//#define MICROPY_MODULE_FROZEN_MPY         (1)
//#define MICROPY_ENABLE_EXTERNAL_IMPORT    (1)

#define MICROPY_ALLOC_PATH_MAX            (256)

// Use the minimum headroom in the chunk allocator for parse nodes.
#define MICROPY_ALLOC_PARSE_CHUNK_INIT    (16)

// scheduler
#define MICROPY_SCHEDULER_DEPTH     (8)
#define MICROPY_SCHEDULER_STATIC_NODES (1)


// type definitions for the specific machine
typedef long mp_off_t;

// We need to provide a declaration/definition of alloca()
#include <alloca.h>

// 启用 readline 历史记录
#define MICROPY_ENABLE_READLINE_HISTORY     (1)
#define MICROPY_READLINE_HISTORY_SIZE       (8)   // 历史记录条数

// 函数声明
void mp_uart_repl_init(void); //REPL初始化 uart_core.c

#define MP_STATE_PORT MP_STATE_VM

// GPIO

// tinyusb
#define MICROPY_HW_ENABLE_USBDEV    (1)
#define MICROPY_HW_TINYUSB_STACK    (1)
#define MICROPY_HW_USB_CDC          (1)
#define CFG_TUD_ENDPPOINT_MAX       (4)
#define CFG_TUSB_MCU                OPT_MCU_RAXXX
#define MICROPY_WRAP_TUD_CDC_RX_CB(name)         __mp_##name
#define MICROPY_WRAP_TUD_CDC_LINE_STATE_CB(name)  __mp_##name
#define MICROPY_WRAP_TUD_EVENT_HOOK_CB(name)     __mp_##name
//#define MICROPY_HW_USB_VID          (0x1209)
//#define MICROPY_HW_USB_PID          (0xABD1)

#define MICROPY_HW_USB_VID          (0x37C5)
#define MICROPY_HW_USB_PID          (0x1204)
// OpenMV
#define MICROPY_FLOAT_IMPL (MICROPY_FLOAT_IMPL_DOUBLE)  // 浮点数
#define CMSIS_MCU_H  "../../../../ra/fsp/src/bsp/cmsis/Device/RENESAS/Include/R7KA8P1KF_core0.h"
#define OMV_USB_STACK_TINYUSB   (1) // tinyusb启用
#define MICROPY_ENABLE_VM_ABORT (1)
#define MICROPY_PY_CSI          (1) // 摄像头


#if MICROPY_PY_THREAD
#define MICROPY_EVENT_POLL_HOOK \
    do {

    } while (0);

#define MICROPY_THREAD_YIELD() pyb_thread_yield()
#else
#define MICROPY_EVENT_POLL_HOOK \
    do { \
        extern void mp_handle_pending(bool); \
        mp_handle_pending(true); \
        __WFI(); \
    } while (0);
#endif

#endif

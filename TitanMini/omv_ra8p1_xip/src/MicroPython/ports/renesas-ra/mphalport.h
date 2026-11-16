#ifndef MICROPYTHON_PORTS_RENESAS_MPHALPORT_H_
#define MICROPYTHON_PORTS_RENESAS_MPHALPORT_H_
#include "irq.h"
#include "modmachine.h"
#include "drivers/fsp_gpio.h"
#include "shared/runtime/interrupt_char.h"
#include "py/ringbuf.h"

#define MICROPY_PY_PENDSV_ENTER   uint32_t atomic_state = raise_irq_pri(IRQ_PRI_PENDSV)
#define MICROPY_PY_PENDSV_EXIT    restore_irq_pri(atomic_state)


#define mp_hal_pin_obj_t        const machine_pin_obj_t *
//#define mp_hal_get_pin_obj(o)   //未实现
//#define mp_hal_pin_name(p)      //未实现
#define mp_hal_pin_input(p)     fsp_gpio_config(p, IOPORT_CFG_PORT_DIRECTION_INPUT)
#define mp_hal_pin_output(p)    fsp_gpio_config(p, IOPORT_CFG_PORT_DIRECTION_OUTPUT)
//#define mp_hal_pin_open_drain(p)    ra_gpio_config((p)->pin, MP_HAL_PIN_MODE_OPEN_DRAIN, MP_HAL_PIN_PULL_NONE, MP_HAL_PIN_DRIVE_0, 0)
#define mp_hal_pin_high(p)      fsp_gpio_write(p, 1)
#define mp_hal_pin_low(p)       fsp_gpio_write(p, 0)
#define mp_hal_pin_toggle(p)    fsp_gpio_toggle(p)
//#define mp_hal_pin_od_low(p)    mp_hal_pin_low(p)
//#define mp_hal_pin_od_high(p)   mp_hal_pin_high(p)
#define mp_hal_pin_read(p)      fsp_gpio_read(p)
#define mp_hal_pin_write(p, v)  do { if (v) { mp_hal_pin_high(p); } else { mp_hal_pin_low(p); } } while (0)

extern ringbuf_t stdin_ringbuf;
extern int mp_interrupt_char;

static inline void mp_hal_wake_main_task_from_isr(void) {
}

// 真正的临界区。此前这里没有定义，于是 py/mphal.h 的默认实现生效：
//     #define MICROPY_BEGIN_ATOMIC_SECTION()  (0)
// 也就是【什么都没做】。于是 mp_sched_schedule_node()、软定时器堆的操作在本
// 线程与其它任务/ISR 之间没有任何互斥。本工程现在多了一个软定时器服务任务，
// 必须补上，否则就是数据竞争。
// 单核 Cortex-M 上禁用 PRIMASK 同时挡住 PendSV，所以对「任务 vs 任务」也是
// 完整的临界区。保存/恢复原值以支持嵌套。
static inline uint32_t mp_hal_atomic_enter(void) {
    uint32_t state = __get_PRIMASK();
    __disable_irq();
    return state;
}

static inline void mp_hal_atomic_exit(uint32_t state) {
    __set_PRIMASK(state);
}

#define MICROPY_BEGIN_ATOMIC_SECTION()     mp_hal_atomic_enter()
#define MICROPY_END_ATOMIC_SECTION(state)  mp_hal_atomic_exit(state)


#endif

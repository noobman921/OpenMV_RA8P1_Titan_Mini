/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2013, 2014 Damien P. George
 * Copyright (c) 2021 Renesas Electronics Corporation
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
 */

#include "py/runtime.h"
#include "py/mphal.h"
#include "shared/runtime/softtimer.h"
#include "systick.h"
#include "hal_data.h"
#include "FreeRTOS.h"
#include "task.h"

// 裸机
//void SysTick_Handler(void) {
//    // Instead of calling HAL_IncTick we do the increment here of the counter.
//    // This is purely for efficiency, since SysTick is called 1000 times per
//    // second at the highest interrupt priority.
//    uint32_t uw_tick = uwTick + 1;
//    uwTick = uw_tick;
//
//    // Read the systick control register. This has the side effect of clearing
//    // the COUNTFLAG bit, which makes the logic in mp_hal_ticks_us
//    // work properly.
//    SysTick->CTRL;
//
//}


// 裸机
//// Core delay function that does an efficient sleep and may switch thread context.
//void mp_hal_delay_ms(mp_uint_t ms) {
//	// use systick counter to do the delay
//	uint32_t start = uwTick;
//	mp_uint_t elapsed = 0;
//	// 让出线程，未实现
//	do {
//		mp_event_wait_ms(ms - elapsed);
//		elapsed = uwTick - start;
//	} while (elapsed < ms);
//}


// 裸机
//mp_uint_t mp_hal_ticks_ms(void) {
//    return uwTick;
//}

#include "FreeRTOS.h"
#include "task.h"

void mp_hal_delay_ms(mp_uint_t ms) {
  vTaskDelay(pdMS_TO_TICKS(ms));
}

void mp_hal_delay_us(mp_uint_t us) {
	// 未实现
}

mp_uint_t mp_hal_ticks_ms(void) {
    return xTaskGetTickCount();   // 1 tick = 1ms
}

/* ----------------------------------------------------------------------------
 * 软定时器的时间基与调度接口
 * ----------------------------------------------------------------------------
 * 对应 Titan_Mini_Openmv/platform/micropython/mphal.c:90-91。
 *
 * 这里【故意不定义】MICROPY_SOFT_TIMER_TICKS_MS：一旦定义，softtimer.c 会把它
 * 当成【变量】使用（`extern __IO uint32_t MICROPY_SOFT_TIMER_TICKS_MS;`，
 * shared/runtime/softtimer.c:36），于是我们又被迫维护一个需要别人持续递增的
 * `uwTick`。而 uwTick 已经害过两次 —— 驱动它的任务一旦被饿死，时间基就冻结
 * （实测冻结 50 秒），所有软定时器全部停摆，包括协议轮询和 VM 节拍。
 *
 * 不定义它，改由移植层提供下面两个函数，时间基直接取 FreeRTOS 的 tick：
 * **永不冻结**，即使驱动 soft_timer_handler() 的任务被短暂延迟。
 * 这正是 Titan 的做法（它的 soft_timer_get_ms() 就是 mp_hal_ticks_ms()）。
 * --------------------------------------------------------------------------*/

uint32_t soft_timer_get_ms(void) {
    return (uint32_t) xTaskGetTickCount();   /* 1 tick = 1 ms */
}

/* svc 任务每 1ms 无条件调用 soft_timer_handler()，所以无需在这里安排唤醒。 */
void soft_timer_schedule_at_ms(uint32_t ticks_ms) {
    (void) ticks_ms;
}

mp_uint_t mp_hal_ticks_us(void) {
	taskENTER_CRITICAL();

    uint32_t ticks = xTaskGetTickCount();
    uint32_t pending = SCB->ICSR & SCB_ICSR_PENDSTSET_Msk;
    uint32_t count = SysTick->VAL;
    uint32_t pending_after = SCB->ICSR & SCB_ICSR_PENDSTSET_Msk;
    if (pending != pending_after) {
        count = SysTick->VAL;
    }
    if (pending_after) {
        ticks++;
    }
    uint32_t elapsed = SysTick->LOAD - count;

    taskEXIT_CRITICAL();

    return (mp_uint_t)((uint64_t)ticks * 1000000 / configTICK_RATE_HZ
                      + (uint64_t)elapsed * 1000000 / SystemCoreClock);
}

mp_uint_t mp_hal_ticks_cpu(void) {
	// 未实现
}

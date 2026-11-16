/*
 * This file is part of the OpenMV project.
 *
 * Copyright (c) 2023 Ibrahim Abdelkader <iabdalkader@openmv.io>
 * Copyright (c) 2023 Kwabena W. Agyeman <kwagyeman@openmv.io>
 *
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 * GPIO port for rp2.
 */
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "omv_boardconfig.h"
#include "omv_gpio.h"

void omv_gpio_config(omv_gpio_t pin, uint32_t mode, uint32_t pull, uint32_t speed, uint32_t af) {
    uint32_t cfg = 0;
    switch(mode) {
        case OMV_GPIO_MODE_INPUT:
            cfg |= IOPORT_CFG_PORT_DIRECTION_INPUT ;
            break;
        case OMV_GPIO_MODE_OUTPUT :
            cfg |= IOPORT_CFG_PORT_DIRECTION_OUTPUT ;
            break;
    }
    switch(pull){
        case OMV_GPIO_PULL_UP:
            cfg |= IOPORT_CFG_PULLUP_ENABLE;
    }
    switch(speed){
        case OMV_GPIO_SPEED_LOW:
            break;
        case OMV_GPIO_SPEED_MED:
            cfg |= IOPORT_CFG_DRIVE_MID;
            break;
        case OMV_GPIO_SPEED_HIGH:
            cfg |= IOPORT_CFG_DRIVE_HIGH;
            break;
        case OMV_GPIO_SPEED_MAX:
            cfg |= IOPORT_CFG_DRIVE_HS_HIGH;
            break;
    }
	R_IOPORT_PinCfg(&g_ioport_ctrl, pin, cfg);

}

void omv_gpio_deinit(omv_gpio_t pin) {
    //无法关闭单个io
}

bool omv_gpio_read(omv_gpio_t pin) {
	bsp_io_level_t value;
	R_IOPORT_PinRead(&g_ioport_ctrl, pin, &value);
	return value;
}

void omv_gpio_write(omv_gpio_t pin, bool value) {
//    rt_pin_write(pin, value);
	R_IOPORT_PinWrite(&g_ioport_ctrl, pin, value);
}

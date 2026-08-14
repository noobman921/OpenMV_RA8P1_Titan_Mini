/*
 * This file is part of the OpenMV project.
 *
 * Copyright (c) 2013-2021 Ibrahim Abdelkader <iabdalkader@openmv.io>
 * Copyright (c) 2013-2021 Kwabena W. Agyeman <kwagyeman@openmv.io>
 *
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 * Board configuration and pin definitions.
 */
#ifndef __OMV_BOARDCONFIG_H__
#define __OMV_BOARDCONFIG_H__


#include "hal_data.h"

// Architecture info
#define OMV_BOARD_ARCH          "RA8P1"
#define OMV_BOARD_TYPE          "M85"

#define OMV_BOARD_UID_ADDR      0x02F07B00
#define OMV_BOARD_UID_SIZE      4
#define OMV_BOARD_UID_OFFSET    0

#define OMV_XCLK_MCO            (0U)
#define OMV_XCLK_TIM            (1U)


// Sensor
// TODO Not actually used right now, frequency is hardcoded.
#define OMV_CSI_CLK_FREQUENCY   (24000000)
#define OMV_GPT_TIM_HANDLE      g_timer11_ctrl
#define OMV_GPT_TIM_CFG         g_timer11_cfg
#define OMV_GPT_PIN             GPT_IO_PIN_GTIOCB

// Sensor IIC Bus
#define OMV_I2C_HANDLE          omv_i2c_ctrl
#define OMV_I2C_CFG             omv_i2c_cfg
#define OMV_CSI_I2C_ID          (0)
#define OMV_CSI_I2C_SPEED       (OMV_I2C_SPEED_STANDARD)
//#define OMV_CAM_BUS_NAME        SENSOR_BUS_NAME
//#define OMV_CAM_BUS_SCL_PIN     BSP_I2C1_SCL_PIN
//#define OMV_CAM_BUS_SDA_PIN     BSP_I2C1_SDA_PIN

// Sensor PLL register value.
#define OMV_OV7725_PLL_CONFIG   (0x01)  // bypass

// Sensor Banding Filter Value
#define OMV_OV7725_BANDING      (0x7F)

// Enable sensor drivers
#define OMV_OV5640_ENABLE       (1)
#define OMV_OV5640_AF_ENABLE    (0)
#define OMV_OV5640_CLK_FREQ    (24000000)
#define OMV_OV5640_PLL_CTRL2    (0x8C)  // MIPI: PLL multiplier=140 (titan验证值, ek示例为0x7B=123)
#define OMV_OV5640_PLL_CTRL3    (0x23)  // MIPI: pll_root_div=2, pll_pre_div=3 (titan验证值, ek示例为0x08)
//#define OMV_OV5640_REV_Y_CHECK  (0)
//#define OMV_OV5640_REV_Y_FREQ   (25000000)
//#define OMV_OV5640_REV_Y_CTRL2  (0x54)
//#define OMV_OV5640_REV_Y_CTRL3  (0x13)

// Enable hardware JPEG
#define OMV_HARDWARE_JPEG       (0)



// Set which OV767x sensor is used
#define OMV_OV7670_VERSION      (70)

// OV7670 clock divider
#define OMV_OV7670_CLKRC        (0x00)

// FIR Module

// Enable WiFi debug
#define OMV_ENABLE_WIFIDBG      (0)
#define OMV_ENABLE_TUSBDBG      (1)
#ifdef PKG_TINYUSB_FULL_SPEED
#define OMV_TUSBDBG_PACKET      (64)
#else
#define OMV_TUSBDBG_PACKET      (512)
#endif

// Enable self-tests on first boot
#define OMV_ENABLE_SELFTEST     (0)

// If buffer size is bigger than this threshold, the quality is reduced.
// This is only used for JPEG images sent to the IDE not normal compression.
#define JPEG_QUALITY_THRESH     (320 * 240 * 2)

// Low and high JPEG QS.
#define OMV_JPEG_QUALITY_LOW        50
#define OMV_JPEG_QUALITY_HIGH       90
#define OMV_JPEG_QUALITY_THRESHOLD  (1920 * 1080 * 2)

// FB Heap Block Size
#define OMV_UMM_BLOCK_SIZE      16

// Core VBAT for selftests
#define OMV_CORE_VBAT           "3.3"

// Buffer size
#define OMV_FB_OVERLAY_MEMORY   SDRAM
#define OMV_JPEG_BUF_SIZE       (OPENMV_JPEG_BUF_SIZE)  // IDE JPEG buffer (header + data).
#define OMV_HEAP_SIZE           (OPENMV_HEAP_SIZE)      // MicroPython's heap

// Main image sensor I2C bus
#define ISC_I2C_ID              (1)
#define ISC_I2C_SPEED           (OMV_I2C_SPEED_STANDARD)

// LCD config.

// WiFi/NINA config.

// AUDIO config.

// VIN (MIPI CSI) config
#define OMV_VIN_HANDLE           g_cam_vin_ctrl
#define OMV_VIN_CFG_             g_cam_vin_cfg
#define OMV_VIN_CFG_EXTEND_      g_cam_vin_cfg_extend
#define OMV_CSI_RESET_PIN        BSP_IO_PORT_11_PIN_00
#define OMV_CSI_POWER_PIN        BSP_IO_PORT_07_PIN_10
extern vin_extended_cfg_t g_cam_vin_cfg_extend;
// // 暂时关闭
//  #define OMV_PROTOCOL_DEFAULT_CHANNELS (0)
#endif //__OMV_BOARDCONFIG_H__

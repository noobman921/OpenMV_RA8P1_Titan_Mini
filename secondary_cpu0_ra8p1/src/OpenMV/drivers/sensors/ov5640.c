/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (C) 2013-2024 OpenMV, LLC.
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
 * OV5640 driver.
 */
#include "omv_boardconfig.h"
#if (OMV_OV5640_ENABLE == 1)

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> // M_LN10 / expf / log10f

#include "omv_i2c.h"
#include "omv_csi.h"
#include "ov5640.h"
#include "py/mphal.h"

#define BLANK_LINES             8
#define DUMMY_LINES             6

#define BLANK_COLUMNS           0
#define DUMMY_COLUMNS           16

#define SENSOR_WIDTH            2624
#define SENSOR_HEIGHT           1964

#define ACTIVE_SENSOR_WIDTH     (SENSOR_WIDTH - BLANK_COLUMNS - (2 * DUMMY_COLUMNS))
#define ACTIVE_SENSOR_HEIGHT    (SENSOR_HEIGHT - BLANK_LINES - (2 * DUMMY_LINES))

#define DUMMY_WIDTH_BUFFER      16
#define DUMMY_HEIGHT_BUFFER     8

#define HSYNC_TIME              252
#define VYSNC_TIME              24

static int16_t readout_x = 0;
static int16_t readout_y = 0;

static uint16_t readout_w = ACTIVE_SENSOR_WIDTH;
static uint16_t readout_h = ACTIVE_SENSOR_HEIGHT;

// set_framesize 已 no-op, HTS/VTS 固定用 default_regs 的 proven 值。
// static uint16_t hts_target = 0;

// ============================================================================
// 寄存器表：VGA 640x480。
// 由 VIN 的 YCbCr->RGB 转换 (color_space_convert_bypass=0) 输出 RGB565。
// 结束哨兵 {0x00,0x00,0x00} 必须在数组末尾 (reset() 以 reg[0]==0 停止)。
// ============================================================================
static const uint8_t default_regs[][3] = {

    // ========== reset / power sequence ==========
    { 0x31, 0x03, 0x11 }, // SCCB system control: software reset via SCCB
    { 0x30, 0x08, 0x82 }, // software reset (bit7=1, self-clears)
    // 注: Titan 在 0x82 后 wait 255ms, 再写 0x42/0x03。OV5640 复位自清除,
    // 连续写 0x42 等效于复位完成后进入 power-down, 无需表内 delay。
    { 0x30, 0x08, 0x42 }, // software power down (bit6=1) + MIPI mode
    { 0x31, 0x03, 0x03 }, // SCCB back to normal mode

    // ========== DVP pins as input ==========
    // DVP 输出若保持使能会驱动 PCLK, 阻止 MIPI PHY 锁定。这组是经典缺失项。
    { 0x30, 0x17, 0x00 }, // DVP: Frex/Vsync/Href/PCLK/D[9:6] -> INPUT
    { 0x30, 0x18, 0x00 }, // DVP: D[5:0]/GPIO[1:0] -> INPUT
    { 0x30, 0x2c, 0xc2 }, // bit1 FREX enable, bit6-7 output drive

    // ========== PLL / PHY mode ==========
    { 0x30, 0x34, 0x18 }, // PLL control0: MIPI 8-bit mode
    { 0x30, 0x35, 0x12 }, // PLL: sys_div=1, mipi_div=2
    { 0x30, 0x36, 0x8c }, // PLL: multiplier=140
    { 0x30, 0x37, 0x13 }, // PLL: root_div=2, pre_div=3
    { 0x31, 0x08, 0x01 }, // system root divider: pclk=1, sclk2x=1, sclk=2

    // ========== MIPI interface enable ==========
    { 0x30, 0x0e, 0x44 }, // MIPI control: 2-lane + MIPI enable   <-- KEY
    { 0x30, 0x2e, 0x08 }, // system control 1 (Titan 紧随 0x300E 写)
    { 0x48, 0x00, 0x24 }, // MIPI Control 00 (per-lane clock setup)
    { 0x30, 0x07, 0xfb }, // Disable DVP PCLK, enable MIPI clock domain  <-- KEY

    // ========== Analog / sensor control ==========
    { 0x36, 0x30, 0x2e }, { 0x36, 0x32, 0xe2 }, { 0x36, 0x33, 0x23 }, { 0x36, 0x21, 0xe0 },
    { 0x37, 0x04, 0xa0 }, { 0x37, 0x03, 0x5a }, { 0x37, 0x15, 0x78 }, { 0x37, 0x17, 0x01 },
    { 0x37, 0x0b, 0x60 }, { 0x37, 0x05, 0x1a }, { 0x39, 0x05, 0x02 }, { 0x39, 0x06, 0x10 },
    { 0x39, 0x01, 0x0a }, { 0x37, 0x31, 0x12 }, { 0x36, 0x00, 0x08 }, { 0x36, 0x01, 0x33 },
    { 0x30, 0x2d, 0x60 }, { 0x36, 0x20, 0x52 }, { 0x37, 0x1b, 0x20 }, { 0x47, 0x1c, 0x50 },

    // ========== AEC / gain ceiling ==========
    { 0x3a, 0x18, 0x00 }, // gain ceiling H
    { 0x3a, 0x19, 0x80 }, // gain ceiling L

    { 0x36, 0x35, 0x1c }, { 0x36, 0x34, 0x40 }, { 0x36, 0x22, 0x01 },

    // ========== 50/60Hz detection thresholds ==========
    { 0x3c, 0x04, 0x28 }, { 0x3c, 0x05, 0x98 }, { 0x3c, 0x06, 0x00 }, { 0x3c, 0x07, 0x08 },
    { 0x3c, 0x08, 0x00 }, { 0x3c, 0x09, 0x1c }, { 0x3c, 0x0a, 0x9c }, { 0x3c, 0x0b, 0x40 },

    // ========== mirror / flip ==========
    { 0x38, 0x20, 0x41 }, // sensor vflip + ISP vflip
    { 0x38, 0x21, 0x01 }, // sensor mirror

    // ========== image window ==========
    // Same as HEAD: 2592x1920, 1/2 subsample -> 1296x960 -> ISP scale(2:1) -> 640x480
    // ISP scale V must be 2:1 integer ratio (OV5640 scaler limitation).
    { 0x38, 0x00, 0x00 }, { 0x38, 0x01, 0x20 }, // Xstart = 32
    { 0x38, 0x02, 0x00 }, { 0x38, 0x03, 0x16 }, // Ystart = 22
    { 0x38, 0x04, 0x0a }, { 0x38, 0x05, 0x20 }, // Xend = 2592
    { 0x38, 0x06, 0x07 }, { 0x38, 0x07, 0x96 }, // Yend = 1942
    { 0x38, 0x10, 0x00 }, { 0x38, 0x11, 0x10 }, // pre-scale X offset = 16
    { 0x38, 0x12, 0x00 }, { 0x38, 0x13, 0x10 }, // pre-scale Y offset = 16

    // ========== output size 640x480 (VGA) ==========
    { 0x38, 0x08, 0x02 }, { 0x38, 0x09, 0x80 }, // Xout = 0x0280 = 640
    { 0x38, 0x0a, 0x01 }, { 0x38, 0x0b, 0xe0 }, // Yout = 0x01E0 = 480

    // ========== subsample increment ==========
    { 0x38, 0x14, 0x31 }, // horizontal subsample increment
    { 0x38, 0x15, 0x31 }, // vertical subsample increment

    // ========== timing / clock enables ==========
    { 0x37, 0x08, 0x64 }, // sensor timing B50
    { 0x40, 0x01, 0x02 }, // BLC start line
    { 0x40, 0x05, 0x1a }, // BLC always update
    { 0x30, 0x00, 0x00 }, // system reset control 0
    { 0x30, 0x02, 0x1c }, // system reset control 2
    { 0x30, 0x04, 0xff }, // clock enable 0
    { 0x30, 0x06, 0xc3 }, // clock enable 2

    // ========== output format: YUV422 ==========
    // 传感器只出 YUV422; RGB565 由 VIN 的 YCbCr->RGB 转换产生。
    { 0x43, 0x00, 0x32 }, // format: YUV422 8-bit, YUYV order
    { 0x50, 0x1f, 0x00 }, // format MUX: ISP YUV422 output (Titan)

    // ========== ISP on (produces valid image) ==========
    { 0x44, 0x07, 0x04 }, // JPEG QS
    { 0x50, 0x00, 0x27 }, // ISP: BPC/WPC/CIP enable, Lenc+AWB module off (对齐上游)

    // ========== AWB manual gains ==========
    { 0x34, 0x06, 0x01 }, // AWB gain manual enable
    { 0x34, 0x00, 0x06 }, { 0x34, 0x01, 0x80 }, // R gain
    { 0x34, 0x02, 0x04 }, { 0x34, 0x03, 0x00 }, // G gain
    { 0x34, 0x04, 0x06 }, { 0x34, 0x05, 0x00 }, // B gain

    // ========== AWB coefficient table ==========
    { 0x51, 0x80, 0xff }, { 0x51, 0x81, 0xf2 }, { 0x51, 0x82, 0x00 }, { 0x51, 0x83, 0x14 },
    { 0x51, 0x84, 0x25 }, { 0x51, 0x85, 0x24 }, { 0x51, 0x86, 0x16 }, { 0x51, 0x87, 0x16 },
    { 0x51, 0x88, 0x16 }, { 0x51, 0x89, 0x62 }, { 0x51, 0x8a, 0x62 }, { 0x51, 0x8b, 0xf0 },
    { 0x51, 0x8c, 0xb2 }, { 0x51, 0x8d, 0x50 }, { 0x51, 0x8e, 0x30 }, { 0x51, 0x8f, 0x30 },
    { 0x51, 0x90, 0x50 }, { 0x51, 0x91, 0xf8 }, { 0x51, 0x92, 0x04 }, { 0x51, 0x93, 0x70 },
    { 0x51, 0x94, 0xf0 }, { 0x51, 0x95, 0xf0 }, { 0x51, 0x96, 0x03 }, { 0x51, 0x97, 0x01 },
    { 0x51, 0x98, 0x04 }, { 0x51, 0x99, 0x12 }, { 0x51, 0x9a, 0x04 }, { 0x51, 0x9b, 0x00 },
    { 0x51, 0x9c, 0x06 }, { 0x51, 0x9d, 0x82 }, { 0x51, 0x9e, 0x38 },

    // ========== color matrix ==========
    { 0x53, 0x81, 0x1e }, { 0x53, 0x82, 0x5b }, { 0x53, 0x83, 0x14 }, { 0x53, 0x84, 0x06 },
    { 0x53, 0x85, 0x82 }, { 0x53, 0x86, 0x88 }, { 0x53, 0x87, 0x7c }, { 0x53, 0x88, 0x60 },
    { 0x53, 0x89, 0x1c }, { 0x53, 0x8a, 0x01 }, { 0x53, 0x8b, 0x98 },

    // ========== sharpness / noise  ==========
    { 0x53, 0x00, 0x08 }, { 0x53, 0x01, 0x30 }, { 0x53, 0x02, 0x3f }, { 0x53, 0x03, 0x10 },
    { 0x53, 0x04, 0x08 }, { 0x53, 0x05, 0x30 }, { 0x53, 0x06, 0x18 }, { 0x53, 0x07, 0x28 },
    { 0x53, 0x09, 0x08 }, { 0x53, 0x0a, 0x30 }, { 0x53, 0x0b, 0x04 }, { 0x53, 0x0c, 0x06 },

    // ========== gamma table ==========
    { 0x54, 0x80, 0x01 }, { 0x54, 0x81, 0x06 }, { 0x54, 0x82, 0x12 }, { 0x54, 0x83, 0x24 },
    { 0x54, 0x84, 0x4a }, { 0x54, 0x85, 0x58 }, { 0x54, 0x86, 0x65 }, { 0x54, 0x87, 0x72 },
    { 0x54, 0x88, 0x7d }, { 0x54, 0x89, 0x88 }, { 0x54, 0x8a, 0x92 }, { 0x54, 0x8b, 0xa3 },
    { 0x54, 0x8c, 0xb2 }, { 0x54, 0x8d, 0xc8 }, { 0x54, 0x8e, 0xdd }, { 0x54, 0x8f, 0xf0 },
    { 0x54, 0x90, 0x15 },

    // ========== UV adjust / brightness / contrast ==========
    { 0x55, 0x80, 0x06 }, { 0x55, 0x83, 0x40 }, { 0x55, 0x84, 0x20 }, { 0x55, 0x89, 0x10 },
    { 0x55, 0x8a, 0x00 }, { 0x55, 0x8b, 0xf8 },
    { 0x50, 0x1d, 0x40 },

    // ========== lens shading correction ==========
    { 0x50, 0x00, 0x27 }, { 0x58, 0x00, 0x20 }, { 0x58, 0x01, 0x19 }, { 0x58, 0x02, 0x17 },
    { 0x58, 0x03, 0x16 }, { 0x58, 0x04, 0x18 }, { 0x58, 0x05, 0x21 }, { 0x58, 0x06, 0x0F },
    { 0x58, 0x07, 0x0A }, { 0x58, 0x08, 0x07 }, { 0x58, 0x09, 0x07 }, { 0x58, 0x0a, 0x0A },
    { 0x58, 0x0b, 0x0C }, { 0x58, 0x0c, 0x0A }, { 0x58, 0x0d, 0x03 }, { 0x58, 0x0e, 0x01 },
    { 0x58, 0x0f, 0x01 }, { 0x58, 0x10, 0x03 }, { 0x58, 0x11, 0x09 }, { 0x58, 0x12, 0x0A },
    { 0x58, 0x13, 0x03 }, { 0x58, 0x14, 0x01 }, { 0x58, 0x15, 0x01 }, { 0x58, 0x16, 0x03 },
    { 0x58, 0x17, 0x08 }, { 0x58, 0x18, 0x10 }, { 0x58, 0x19, 0x0A }, { 0x58, 0x1a, 0x06 },
    { 0x58, 0x1b, 0x06 }, { 0x58, 0x1c, 0x08 }, { 0x58, 0x1d, 0x0E }, { 0x58, 0x1e, 0x22 },
    { 0x58, 0x1f, 0x18 }, { 0x58, 0x20, 0x13 }, { 0x58, 0x21, 0x12 }, { 0x58, 0x22, 0x16 },
    { 0x58, 0x23, 0x1E }, { 0x58, 0x24, 0x64 }, { 0x58, 0x25, 0x2A }, { 0x58, 0x26, 0x2C },
    { 0x58, 0x27, 0x2A }, { 0x58, 0x28, 0x46 }, { 0x58, 0x29, 0x2A }, { 0x58, 0x2a, 0x26 },
    { 0x58, 0x2b, 0x24 }, { 0x58, 0x2c, 0x26 }, { 0x58, 0x2d, 0x2A }, { 0x58, 0x2e, 0x28 },
    { 0x58, 0x2f, 0x42 }, { 0x58, 0x30, 0x40 }, { 0x58, 0x31, 0x42 }, { 0x58, 0x32, 0x08 },
    { 0x58, 0x33, 0x28 }, { 0x58, 0x34, 0x26 }, { 0x58, 0x35, 0x24 }, { 0x58, 0x36, 0x26 },
    { 0x58, 0x37, 0x2A }, { 0x58, 0x38, 0x44 }, { 0x58, 0x39, 0x4A }, { 0x58, 0x3a, 0x2C },
    { 0x58, 0x3b, 0x2a }, { 0x58, 0x3c, 0x46 }, { 0x58, 0x3d, 0xCE },

    // ========== AVG table ==========
    // AVG 权重影响 AWB auto 的 R/G/B 增益计算; Titan 权重导致 AWB 压 B(白纸 B=8 vs R=26)。
    // 上游全用 0x11(均匀权重), 对齐后 AWB 应能正确平衡三通道。
    { 0x56, 0x88, 0x11 }, { 0x56, 0x89, 0x11 }, { 0x56, 0x8a, 0x11 }, { 0x56, 0x8b, 0x11 },
    { 0x56, 0x8c, 0x11 }, { 0x56, 0x8d, 0x11 }, { 0x56, 0x8e, 0x11 }, { 0x56, 0x8f, 0x11 },
    { 0x50, 0x25, 0x00 },

    // ========== AEC target ==========
    { 0x3a, 0x0f, 0x40 }, { 0x3a, 0x10, 0x30 }, { 0x3a, 0x1b, 0x40 }, { 0x3a, 0x1e, 0x30 },
    { 0x3a, 0x11, 0x71 }, { 0x3a, 0x1f, 0x20 },

    // ========== MIPI control / timing ==========
    { 0x48, 0x00, 0x24 }, // MIPI Control 00
    { 0x30, 0x07, 0xfb }, // Disable DVP PCLK, enable MIPI clock domain

    // ========== frame timing HTS/VTS ==========
    { 0x38, 0x0c, 0x06 }, { 0x38, 0x0d, 0x40 }, // HTS = 0x0640 = 1600
    { 0x38, 0x0e, 0x03 }, { 0x38, 0x0f, 0xd8 }, // VTS = 0x03D8 = 984

    // ========== 50/60Hz detector ==========
    { 0x3c, 0x01, 0xb4 }, { 0x3c, 0x00, 0x04 },
    { 0x3a, 0x08, 0x00 }, { 0x3a, 0x09, 0x93 },
    { 0x3a, 0x0e, 0x06 }, { 0x3a, 0x0a, 0x00 }, { 0x3a, 0x0b, 0x7b }, { 0x3a, 0x0d, 0x08 },

    // ========== AEC/AGC power down domain control ==========
    { 0x3a, 0x00, 0x3c }, { 0x3a, 0x02, 0x05 }, { 0x3a, 0x03, 0x44 }, { 0x3a, 0x14, 0x05 },
    { 0x3a, 0x15, 0x44 },

    // ========== misc ==========
    { 0x36, 0x18, 0x00 }, { 0x36, 0x12, 0x29 }, { 0x37, 0x08, 0x64 }, { 0x37, 0x09, 0x52 },
    { 0x37, 0x0c, 0x03 },

    // ========== BLC ==========
    { 0x40, 0x01, 0x02 }, { 0x40, 0x04, 0x02 }, { 0x40, 0x05, 0x1a },

    // ========== output data path ==========
    { 0x47, 0x13, 0x03 }, // JPEG mode select
    { 0x46, 0x0b, 0x35 }, // VFIFO
    { 0x46, 0x0c, 0x22 }, // VFIFO PCLK manual
    { 0x48, 0x37, 0x0a }, // MIPI global timing
    { 0x38, 0x24, 0x01 }, // MIPI timing tweak
    { 0x50, 0x01, 0xa3 }, // ISP: AWB/color matrix/UV/scale(bit5=1,enabled)/SDE enable

    // ========== AWB auto ==========
    // 上方 0x680/0x400/0x600 作为 AWB 起点, 由传感器自动调整
    { 0x34, 0x06, 0x00 }, // AWB gain manual DISABLE (auto)

    // ========== 48fps: manual exposure = 984 lines, AGC auto ==========
    // 20-bit AEC_PK_EXPOSURE (4-bit fraction): value = 984 << 4 = 0x3D80
    { 0x35, 0x03, 0x01 }, // AEC_PK_MANUAL: bit0=1 manual exposure, bit1=0 AGC auto
    { 0x35, 0x00, 0x03 }, // AEC_PK_EXPOSURE[19:12]
    { 0x35, 0x01, 0xd8 }, // AEC_PK_EXPOSURE[11:4]
    { 0x35, 0x02, 0x00 }, // AEC_PK_EXPOSURE[3:0]

    // ========== wake up ==========
    { 0x30, 0x08, 0x02 }, // wake up (clear power-down bit6)

    // ========== disable test patterns ==========
    { 0x50, 0x3d, 0x00 }, // ISP test pattern off
    { 0x47, 0x41, 0x00 }, // test pattern color bar off

    // ========== MIPI virtual channel 0 ==========
    { 0x48, 0x14, 0x00 }, // MIPI CSI control: VC=0

    // End.
    { 0x00, 0x00, 0x00 }
};

#define NUM_BRIGHTNESS_LEVELS    (9)

#define NUM_CONTRAST_LEVELS      (7)
static const uint8_t contrast_regs[NUM_CONTRAST_LEVELS][1] = {
    {0x14}, /* -3 */
    {0x18}, /* -2 */
    {0x1C}, /* -1 */
    {0x00}, /* +0 */
    {0x10}, /* +1 */
    {0x18}, /* +2 */
    {0x1C}, /* +3 */
};

#define NUM_SATURATION_LEVELS    (7)
static const uint8_t saturation_regs[NUM_SATURATION_LEVELS][6] = {
    {0x0c, 0x30, 0x3d, 0x3e, 0x3d, 0x01}, /* -3 */
    {0x10, 0x3d, 0x4d, 0x4e, 0x4d, 0x01}, /* -2 */
    {0x15, 0x52, 0x66, 0x68, 0x66, 0x02}, /* -1 */
    {0x1a, 0x66, 0x80, 0x82, 0x80, 0x02}, /* +0 */
    {0x1f, 0x7a, 0x9a, 0x9c, 0x9a, 0x02}, /* +1 */
    {0x24, 0x8f, 0xb3, 0xb6, 0xb3, 0x03}, /* +2 */
    {0x2b, 0xab, 0xd6, 0xda, 0xd6, 0x04}, /* +3 */
};

static int reset(omv_csi_t *csi) {
    int ret = 0;
    readout_x = 0;
    readout_y = 0;

    readout_w = ACTIVE_SENSOR_WIDTH;
    readout_h = ACTIVE_SENSOR_HEIGHT;

    // [RGB_TEST] set_framesize 已 no-op, HTS/VTS 固定用 default_regs 值。
    // hts_target = 0;

    // Reset all registers
    ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, SCCB_SYSTEM_CTRL_1, 2, 0x11, 1);
    ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, SYSTEM_CTROL0, 2, 0x82, 1);

    // [Titan 对齐] 软件复位后等 255ms, 让 SCCB/时钟稳定再配置 MIPI (Titan 在
    // 0x82 后 wait 255ms 才继续, 5ms 太短可能导致传感器未就绪)。
    mp_hal_delay_ms(255);

    // Write default registers
    for (int i = 0; default_regs[i][0]; i++) {
        int addr = (default_regs[i][0] << 8) | (default_regs[i][1] << 0);
        int data = default_regs[i][2];

        ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, addr, 2, data, 1);
    }

    // Delay 300 ms
    if (!csi->disable_delays) {
        mp_hal_delay_ms(300);
    }

    return ret;
}

static int sleep(omv_csi_t *csi, int enable) {
    uint8_t reg;
    if (enable) {
        reg = 0x42;
    } else {
        reg = 0x02;
    }

    return omv_i2c_write_reg(csi->i2c, csi->slv_addr, SYSTEM_CTROL0, 2, reg, 1);
}

static int read_reg(omv_csi_t *csi, uint16_t reg_addr) {
    uint8_t reg_data;
    if (omv_i2c_read_reg(csi->i2c, csi->slv_addr, reg_addr, 2, &reg_data, 1) != 0) {
        return -1;
    }
    return reg_data;
}

static int write_reg(omv_csi_t *csi, uint16_t reg_addr, uint16_t reg_data) {
    return omv_i2c_write_reg(csi->i2c, csi->slv_addr, reg_addr, 2, reg_data, 1);
}

// [RGB_TEST] HTS/VTS 固定用 default_regs 的 proven 值 (Titan VGA 2128/1200),
// 不再动态计算, calculate_hts/calculate_vts 保留为注释以备后续启用。
/*
static int calculate_hts(omv_csi_t *csi, uint16_t width) {
    uint16_t hts = hts_target;

    if ((csi->pixformat == PIXFORMAT_GRAYSCALE) || (csi->pixformat == PIXFORMAT_BAYER) ||
        (csi->pixformat == PIXFORMAT_JPEG)) {
        if (width <= 1280) {
            hts = IM_MAX((width * 2) + 8, hts_target);
        }
    } else {
        if (width > 640) {
            hts = IM_MAX((width * 2) + 8, hts_target);
        }
    }

    if (width <= 640) {
        hts += 160;               // Fix image quality at low resolutions.

    }
    return IM_MAX(hts + HSYNC_TIME, (SENSOR_WIDTH + HSYNC_TIME) / 2); // Fix to prevent crashing.
}

static int calculate_vts(omv_csi_t *csi, uint16_t readout_height) {
    return IM_MAX(readout_height + VYSNC_TIME, (SENSOR_HEIGHT + VYSNC_TIME) / 8); // Fix to prevent crashing.
}
*/

static int set_pixformat(omv_csi_t *csi, pixformat_t pixformat) {
    // [RGB_TEST] 只靠 default_regs 初始化, 不在此改写传感器格式。
    // 传感器保持 YUV422 (0x4300=0x32, 0x501f=0x00), 与 Titan 一致;
    // RGB565 由 VIN 的 YCbCr->RGB 转换产生 (见 ports/ra8/omv_csi.c)。
    (void) csi;
    (void) pixformat;
    return 0;
}

static int set_framesize(omv_csi_t *csi, omv_csi_framesize_t framesize) {
    // [RGB_TEST] 只靠 default_regs 初始化, 不在此改写传感器窗口/时序。
    // 传感器固定为 Titan VGA (640x480, Xstart=32 Ystart=22, HTS/VTS=2128/1200);
    // 后续由 VIN 缩放输出, 见 ports/ra8/omv_csi.c。
    (void) csi;
    (void) framesize;
    return 0;
}

static int set_contrast(omv_csi_t *csi, int level) {
    int ret = 0;

    int new_level = level + (NUM_CONTRAST_LEVELS / 2);
    if (new_level < 0 || new_level >= NUM_CONTRAST_LEVELS) {
        return -1;
    }

    ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, 0x3212, 2, 0x03, 1); // start group 3
    ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, 0x5586, 2, (new_level + 5) << 2, 1);
    ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, 0x5585, 2, contrast_regs[new_level][0], 1);
    ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, 0x3212, 2, 0x13, 1); // end group 3
    ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, 0x3212, 2, 0xa3, 1); // launch group 3

    return ret;
}

static int set_brightness(omv_csi_t *csi, int level) {
    int ret = 0;

    int new_level = level + (NUM_BRIGHTNESS_LEVELS / 2);
    if (new_level < 0 || new_level >= NUM_BRIGHTNESS_LEVELS) {
        return -1;
    }

    ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, 0x3212, 2, 0x03, 1); // start group 3
    ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, 0x5587, 2, abs(level) << 4, 1);
    ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, 0x5588, 2, (level < 0) ? 0x09 : 0x01, 1);
    ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, 0x3212, 2, 0x13, 1); // end group 3
    ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, 0x3212, 2, 0xa3, 1); // launch group 3

    return ret;
}

static int set_saturation(omv_csi_t *csi, int level) {
    int ret = 0;

    int new_level = level + (NUM_SATURATION_LEVELS / 2);
    if (new_level < 0 || new_level >= NUM_SATURATION_LEVELS) {
        return -1;
    }

    ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, 0x3212, 2, 0x03, 1); // start group 3
    ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, 0x5581, 2, 0x1c, 1);
    ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, 0x5582, 2, 0x5a, 1);
    ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, 0x5583, 2, 0x06, 1);
    ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, 0x5584, 2, saturation_regs[new_level][0], 1);
    ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, 0x5585, 2, saturation_regs[new_level][1], 1);
    ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, 0x5586, 2, saturation_regs[new_level][2], 1);
    ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, 0x5587, 2, saturation_regs[new_level][3], 1);
    ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, 0x5588, 2, saturation_regs[new_level][4], 1);
    ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, 0x5589, 2, saturation_regs[new_level][5], 1);
    ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, 0x558b, 2, 0x98, 1);
    ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, 0x558a, 2, 0x01, 1);
    ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, 0x3212, 2, 0x13, 1); // end group 3
    ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, 0x3212, 2, 0xa3, 1); // launch group 3

    return ret;
}

static int set_gainceiling(omv_csi_t *csi, omv_csi_gainceiling_t gainceiling) {
    uint8_t reg;
    int ret = 0;

    int new_gainceiling = 16 << (gainceiling + 1);
    if (new_gainceiling >= 1024) {
        return -1;
    }

    ret |= omv_i2c_read_reg(csi->i2c, csi->slv_addr, AEC_GAIN_CEILING_H, 2, &reg, 1);
    ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, AEC_GAIN_CEILING_H, 2, (reg & 0xFC) | (new_gainceiling >> 8), 1);
    ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, AEC_GAIN_CEILING_L, 2, new_gainceiling, 1);

    return ret;
}

static int set_quality(omv_csi_t *csi, int qs) {
    uint8_t reg;
    int ret = omv_i2c_read_reg(csi->i2c, csi->slv_addr, JPEG_CTRL07, 2, &reg, 1);
    ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, JPEG_CTRL07, 2, (reg & 0xC0) | (qs >> 2), 1);

    return ret;
}

static int set_colorbar(omv_csi_t *csi, int enable) {
    uint8_t reg;
    int ret = omv_i2c_read_reg(csi->i2c, csi->slv_addr, PRE_ISP_TEST, 2, &reg, 1);
    return omv_i2c_write_reg(csi->i2c, csi->slv_addr, PRE_ISP_TEST, 2, (reg & 0x7F) | (enable ? 0x80 : 0x00), 1) | ret;
}

static int set_auto_gain(omv_csi_t *csi, int enable, float gain_db, float gain_db_ceiling) {
    uint8_t reg;
    int ret = omv_i2c_read_reg(csi->i2c, csi->slv_addr, AEC_PK_MANUAL, 2, &reg, 1);
    ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, AEC_PK_MANUAL, 2, (reg & 0xFD) | ((enable == 0) << 1), 1);

    if ((enable == 0) && (!isnanf(gain_db)) && (!isinff(gain_db))) {
        int gain = __USAT(fast_roundf(expf((gain_db / 20.0f) * M_LN10) * 16.0f), 10);

        ret |= omv_i2c_read_reg(csi->i2c, csi->slv_addr, AEC_PK_REAL_GAIN_H, 2, &reg, 1);
        ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, AEC_PK_REAL_GAIN_H, 2, (reg & 0xFC) | (gain >> 8), 1);
        ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, AEC_PK_REAL_GAIN_L, 2, gain, 1);
    } else if ((enable != 0) && (!isnanf(gain_db_ceiling)) && (!isinff(gain_db_ceiling))) {
        int gain_ceiling = __USAT(fast_roundf(expf((gain_db_ceiling / 20.0f) * M_LN10) * 16.0f), 10);

        ret |= omv_i2c_read_reg(csi->i2c, csi->slv_addr, AEC_GAIN_CEILING_H, 2, &reg, 1);
        ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, AEC_GAIN_CEILING_H, 2, (reg & 0xFC) | (gain_ceiling >> 8), 1);
        ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, AEC_GAIN_CEILING_L, 2, gain_ceiling, 1);
    }

    return ret;
}

static int get_gain_db(omv_csi_t *csi, float *gain_db) {
    uint8_t gainh, gainl;

    int ret = omv_i2c_read_reg(csi->i2c, csi->slv_addr, AEC_PK_REAL_GAIN_H, 2, &gainh, 1);
    ret |= omv_i2c_read_reg(csi->i2c, csi->slv_addr, AEC_PK_REAL_GAIN_L, 2, &gainl, 1);

    *gain_db = 20.0f * log10f((((gainh & 0x3) << 8) | gainl) / 16.0f);

    return ret;
}

static int calc_pclk_freq(omv_csi_t *csi,
                          uint8_t sc_pll_ctrl_0,
                          uint8_t sc_pll_ctrl_1,
                          uint8_t sc_pll_ctrl_2,
                          uint8_t sc_pll_ctrl_3,
                          uint8_t sys_root_div) {
    uint32_t pclk_freq = omv_csi_get_clk_frequency(csi, false);
    pclk_freq /= ((sc_pll_ctrl_3 & 0x10) != 0x00) ? 2 : 1;
    pclk_freq /= ((sc_pll_ctrl_0 & 0x0F) == 0x0A) ? 5 : 4; //camera has two MIPI lanes
    switch (sc_pll_ctrl_3 & 0x0F) {
        case  1: pclk_freq /= 1; break;
        case  2: pclk_freq /= 2; break;
        case  3: pclk_freq /= 3; break;
        case  4: pclk_freq /= 4; break;
        case  6: pclk_freq /= 6; break;
        case  8: pclk_freq /= 8; break;
        default: pclk_freq /= 3; break;
    }
    pclk_freq *= sc_pll_ctrl_2;
    sc_pll_ctrl_1 >>= 4;
    pclk_freq /= sc_pll_ctrl_1;
    switch (sys_root_div & 0x30) {
        case 0x00: pclk_freq /= 1; break;
        case 0x10: pclk_freq /= 2; break;
        case 0x20: pclk_freq /= 4; break;
        case 0x30: pclk_freq /= 8; break;
        default:   pclk_freq /= 1; break;
    }
    return (int) pclk_freq;
}

static int set_auto_exposure(omv_csi_t *csi, int enable, int exposure_us) {
    uint8_t reg, spc0, spc1, spc2, spc3, sysrootdiv, hts_h, hts_l, vts_h, vts_l;
    int ret = omv_i2c_read_reg(csi->i2c, csi->slv_addr, AEC_PK_MANUAL, 2, &reg, 1);
    ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, AEC_PK_MANUAL, 2, (reg & 0xFE) | ((enable == 0) << 0), 1);

    if ((enable == 0) && (exposure_us >= 0)) {
        ret |= omv_i2c_read_reg(csi->i2c, csi->slv_addr, SC_PLL_CONTRL0, 2, &spc0, 1);
        ret |= omv_i2c_read_reg(csi->i2c, csi->slv_addr, SC_PLL_CONTRL1, 2, &spc1, 1);
        ret |= omv_i2c_read_reg(csi->i2c, csi->slv_addr, SC_PLL_CONTRL2, 2, &spc2, 1);
        ret |= omv_i2c_read_reg(csi->i2c, csi->slv_addr, SC_PLL_CONTRL3, 2, &spc3, 1);
        ret |= omv_i2c_read_reg(csi->i2c, csi->slv_addr, SYSTEM_ROOT_DIVIDER, 2, &sysrootdiv, 1);

        ret |= omv_i2c_read_reg(csi->i2c, csi->slv_addr, TIMING_HTS_H, 2, &hts_h, 1);
        ret |= omv_i2c_read_reg(csi->i2c, csi->slv_addr, TIMING_HTS_L, 2, &hts_l, 1);

        ret |= omv_i2c_read_reg(csi->i2c, csi->slv_addr, TIMING_VTS_H, 2, &vts_h, 1);
        ret |= omv_i2c_read_reg(csi->i2c, csi->slv_addr, TIMING_VTS_L, 2, &vts_l, 1);

        uint16_t hts = (hts_h << 8) | hts_l;
        uint16_t vts = (vts_h << 8) | vts_l;

        int pclk_freq = calc_pclk_freq(csi, spc0, spc1, spc2, spc3, sysrootdiv);
        int clocks_per_us = pclk_freq / 1000000;
        int exposure = __USAT((exposure_us * clocks_per_us) / hts, 16);

        int new_vts = IM_MAX(exposure, vts);

        ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, AEC_PK_EXPOSURE_0, 2, exposure >> 12, 1);
        ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, AEC_PK_EXPOSURE_1, 2, exposure >> 4, 1);
        ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, AEC_PK_EXPOSURE_2, 2, exposure << 4, 1);

        ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, TIMING_VTS_H, 2, new_vts >> 8, 1);
        ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, TIMING_VTS_L, 2, new_vts, 1);
    }

    return ret;
}

static int get_exposure_us(omv_csi_t *csi, int *exposure_us) {
    uint8_t spc0, spc1, spc2, spc3, sysrootdiv, aec_0, aec_1, aec_2, hts_h, hts_l, vts_h, vts_l;
    int ret = 0;

    ret |= omv_i2c_read_reg(csi->i2c, csi->slv_addr, SC_PLL_CONTRL0, 2, &spc0, 1);
    ret |= omv_i2c_read_reg(csi->i2c, csi->slv_addr, SC_PLL_CONTRL1, 2, &spc1, 1);
    ret |= omv_i2c_read_reg(csi->i2c, csi->slv_addr, SC_PLL_CONTRL2, 2, &spc2, 1);
    ret |= omv_i2c_read_reg(csi->i2c, csi->slv_addr, SC_PLL_CONTRL3, 2, &spc3, 1);
    ret |= omv_i2c_read_reg(csi->i2c, csi->slv_addr, SYSTEM_ROOT_DIVIDER, 2, &sysrootdiv, 1);

    ret |= omv_i2c_read_reg(csi->i2c, csi->slv_addr, AEC_PK_EXPOSURE_0, 2, &aec_0, 1);
    ret |= omv_i2c_read_reg(csi->i2c, csi->slv_addr, AEC_PK_EXPOSURE_1, 2, &aec_1, 1);
    ret |= omv_i2c_read_reg(csi->i2c, csi->slv_addr, AEC_PK_EXPOSURE_2, 2, &aec_2, 1);

    ret |= omv_i2c_read_reg(csi->i2c, csi->slv_addr, TIMING_HTS_H, 2, &hts_h, 1);
    ret |= omv_i2c_read_reg(csi->i2c, csi->slv_addr, TIMING_HTS_L, 2, &hts_l, 1);

    ret |= omv_i2c_read_reg(csi->i2c, csi->slv_addr, TIMING_VTS_H, 2, &vts_h, 1);
    ret |= omv_i2c_read_reg(csi->i2c, csi->slv_addr, TIMING_VTS_L, 2, &vts_l, 1);

    uint32_t aec = ((aec_0 << 16) | (aec_1 << 8) | aec_2) >> 4;
    uint16_t hts = (hts_h << 8) | hts_l;
    uint16_t vts = (vts_h << 8) | vts_l;

    aec = IM_MIN(aec, vts);

    int pclk_freq = calc_pclk_freq(csi, spc0, spc1, spc2, spc3, sysrootdiv);
    int clocks_per_us = pclk_freq / 1000000;
    *exposure_us = (aec * hts) / clocks_per_us;

    return ret;
}

static int set_auto_whitebal(omv_csi_t *csi, int enable, float r_gain_db, float g_gain_db, float b_gain_db) {
    uint8_t reg;
    int ret = omv_i2c_read_reg(csi->i2c, csi->slv_addr, AWB_MANUAL_CONTROL, 2, &reg, 1);
    ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, AWB_MANUAL_CONTROL, 2, (reg & 0xFE) | (enable == 0), 1);

    if ((enable == 0) && (!isnanf(r_gain_db)) && (!isnanf(g_gain_db)) && (!isnanf(b_gain_db))
        && (!isinff(r_gain_db)) && (!isinff(g_gain_db)) && (!isinff(b_gain_db))) {

        int r_gain = __USAT(fast_roundf(expf((r_gain_db / 20.0f) * M_LN10)), 12);
        int g_gain = __USAT(fast_roundf(expf((g_gain_db / 20.0f) * M_LN10)), 12);
        int b_gain = __USAT(fast_roundf(expf((b_gain_db / 20.0f) * M_LN10)), 12);

        ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, AWB_R_GAIN_H, 2, r_gain >> 8, 1);
        ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, AWB_R_GAIN_L, 2, r_gain, 1);
        ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, AWB_G_GAIN_H, 2, g_gain >> 8, 1);
        ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, AWB_G_GAIN_L, 2, g_gain, 1);
        ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, AWB_B_GAIN_H, 2, b_gain >> 8, 1);
        ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, AWB_B_GAIN_L, 2, b_gain, 1);
    }

    return ret;
}

static int get_rgb_gain_db(omv_csi_t *csi, float *r_gain_db, float *g_gain_db, float *b_gain_db) {
    uint8_t redh, redl, greenh, greenl, blueh, bluel;

    int ret = omv_i2c_read_reg(csi->i2c, csi->slv_addr, AWB_R_GAIN_H, 2, &redh, 1);
    ret |= omv_i2c_read_reg(csi->i2c, csi->slv_addr, AWB_R_GAIN_L, 2, &redl, 1);
    ret |= omv_i2c_read_reg(csi->i2c, csi->slv_addr, AWB_G_GAIN_H, 2, &greenh, 1);
    ret |= omv_i2c_read_reg(csi->i2c, csi->slv_addr, AWB_G_GAIN_L, 2, &greenl, 1);
    ret |= omv_i2c_read_reg(csi->i2c, csi->slv_addr, AWB_B_GAIN_H, 2, &blueh, 1);
    ret |= omv_i2c_read_reg(csi->i2c, csi->slv_addr, AWB_B_GAIN_L, 2, &bluel, 1);

    *r_gain_db = 20.0f * log10f(((redh & 0xF) << 8) | redl);
    *g_gain_db = 20.0f * log10f(((greenh & 0xF) << 8) | greenl);
    *b_gain_db = 20.0f * log10f(((blueh & 0xF) << 8) | bluel);

    return ret;
}

static int set_auto_blc(omv_csi_t *csi, int enable, int *regs) {
    uint8_t reg;
    int ret = omv_i2c_read_reg(csi->i2c, csi->slv_addr, BLC_CTRL_00, 2, &reg, 1);
    ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, BLC_CTRL_00, 2, (reg & 0xFE) | (enable != 0), 1);

    if ((enable == 0) && (regs != NULL)) {
        for (uint32_t i = 0; i < csi->blc_size; i++) {
            ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, BLACK_LEVEL_00_H + i, 2, regs[i], 1);
        }
    }

    return ret;
}

static int get_blc_regs(omv_csi_t *csi, int *regs) {
    int ret = 0;

    for (uint32_t i = 0; i < csi->blc_size; i++) {
        uint8_t reg;
        ret |= omv_i2c_read_reg(csi->i2c, csi->slv_addr, BLACK_LEVEL_00_H + i, 2, &reg, 1);
        regs[i] = reg;
    }

    return ret;
}

static int set_hmirror(omv_csi_t *csi, int enable) {
    uint8_t reg;
    int ret = omv_i2c_read_reg(csi->i2c, csi->slv_addr, TIMING_TC_REG_21, 2, &reg, 1);
    if (enable) {
        ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, TIMING_TC_REG_21, 2, reg | 0x06, 1);
    } else {
        ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, TIMING_TC_REG_21, 2, reg & 0xF9, 1);
    }
    return ret;
}

static int set_vflip(omv_csi_t *csi, int enable) {
    uint8_t reg;
    int ret = omv_i2c_read_reg(csi->i2c, csi->slv_addr, TIMING_TC_REG_20, 2, &reg, 1);
    if (!enable) {
        ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, TIMING_TC_REG_20, 2, reg | 0x06, 1);
    } else {
        ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, TIMING_TC_REG_20, 2, reg & 0xF9, 1);
    }
    return ret;
}

static int set_special_effect(omv_csi_t *csi, omv_csi_sde_t sde) {
    int ret = 0;

    switch (sde) {
        case OMV_CSI_SDE_NEGATIVE:
            ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, 0x3212, 2, 0x03, 1); // start group 3
            ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, 0x5580, 2, 0x40, 1);
            ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, 0x5003, 2, 0x08, 1);
            ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, 0x5583, 2, 0x40, 1); // sat U
            ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, 0x5584, 2, 0x10, 1); // sat V
            ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, 0x3212, 2, 0x13, 1); // end group 3
            ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, 0x3212, 2, 0xa3, 1); // latch group 3
            break;
        case OMV_CSI_SDE_NORMAL:
            ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, 0x3212, 2, 0x03, 1); // start group 3
            ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, 0x5580, 2, 0x06, 1);
            ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, 0x5583, 2, 0x40, 1); // sat U
            ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, 0x5584, 2, 0x10, 1); // sat V
            ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, 0x5003, 2, 0x08, 1);
            ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, 0x3212, 2, 0x13, 1); // end group 3
            ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, 0x3212, 2, 0xa3, 1); // latch group 3
            break;
        default:
            return -1;
    }

    return ret;
}

static int set_lens_correction(omv_csi_t *csi, int enable, int radi, int coef) {
    uint8_t reg;
    int ret = omv_i2c_read_reg(csi->i2c, csi->slv_addr, ISP_CONTROL_00, 2, &reg, 1);
    return omv_i2c_write_reg(csi->i2c, csi->slv_addr, ISP_CONTROL_00, 2, (reg & 0x7F) | (enable ? 0x80 : 0x00), 1) | ret;
}

static int ioctl(omv_csi_t *csi, int request, va_list ap) {
    int ret = 0;
    uint8_t reg;

    switch (request) {
        case OMV_CSI_IOCTL_SET_READOUT_WINDOW: {
            int tmp_readout_x = va_arg(ap, int);
            int tmp_readout_y = va_arg(ap, int);
            int tmp_readout_w = IM_CLAMP(va_arg(ap, int),
                                         csi->resolution[csi->framesize][0],
                                         ACTIVE_SENSOR_WIDTH);
            int tmp_readout_h = IM_CLAMP(va_arg(ap, int),
                                         csi->resolution[csi->framesize][1],
                                         ACTIVE_SENSOR_HEIGHT);
            int readout_x_max = (ACTIVE_SENSOR_WIDTH - tmp_readout_w) / 2;
            int readout_y_max = (ACTIVE_SENSOR_HEIGHT - tmp_readout_h) / 2;
            tmp_readout_x = IM_CLAMP(tmp_readout_x, -readout_x_max, readout_x_max);
            tmp_readout_y = IM_CLAMP(tmp_readout_y, -readout_y_max, readout_y_max);
            bool changed = (tmp_readout_x != readout_x) || (tmp_readout_y != readout_y) || (tmp_readout_w != readout_w) ||
                           (tmp_readout_h != readout_h);
            readout_x = tmp_readout_x;
            readout_y = tmp_readout_y;
            readout_w = tmp_readout_w;
            readout_h = tmp_readout_h;
            if (changed && (csi->framesize != OMV_CSI_FRAMESIZE_INVALID)) {
                set_framesize(csi, csi->framesize);
            }
            break;
        }
        case OMV_CSI_IOCTL_GET_READOUT_WINDOW: {
            *va_arg(ap, int *) = readout_x;
            *va_arg(ap, int *) = readout_y;
            *va_arg(ap, int *) = readout_w;
            *va_arg(ap, int *) = readout_h;
            break;
        }
        case OMV_CSI_IOCTL_SET_NIGHT_MODE: {
            int enable = va_arg(ap, int);
            ret = omv_i2c_read_reg(csi->i2c, csi->slv_addr, AEC_CTRL_00, 2, &reg, 1);
            ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, AEC_CTRL_00, 2,
                                     (reg & 0xFB) | ((enable != 0) << 2), 1);
            break;
        }
        case OMV_CSI_IOCTL_GET_NIGHT_MODE: {
            int *enable = va_arg(ap, int *);
            ret = omv_i2c_read_reg(csi->i2c, csi->slv_addr, AEC_CTRL_00, 2, &reg, 1);
            if (ret >= 0) {
                *enable = reg & 0x4;
            }
            break;
        }
        default: {
            ret = -1;
            break;
        }
    }

    return ret;
}

int ov5640_init(omv_csi_t *csi) {
    // Initialize csi structure.
    csi->reset = reset;
    csi->sleep = sleep;
    csi->read_reg = read_reg;
    csi->write_reg = write_reg;
    csi->set_pixformat = set_pixformat;
    csi->set_framesize = set_framesize;
    csi->set_contrast = set_contrast;
    csi->set_brightness = set_brightness;
    csi->set_saturation = set_saturation;
    csi->set_gainceiling = set_gainceiling;
    csi->set_quality = set_quality;
    csi->set_colorbar = set_colorbar;
    csi->set_auto_gain = set_auto_gain;
    csi->get_gain_db = get_gain_db;
    csi->set_auto_exposure = set_auto_exposure;
    csi->get_exposure_us = get_exposure_us;
    csi->set_auto_whitebal = set_auto_whitebal;
    csi->get_rgb_gain_db = get_rgb_gain_db;
    csi->set_auto_blc = set_auto_blc;
    csi->get_blc_regs = get_blc_regs;
    csi->set_hmirror = set_hmirror;
    csi->set_vflip = set_vflip;
    csi->set_special_effect = set_special_effect;
    csi->set_lens_correction = set_lens_correction;
    csi->ioctl = ioctl;

    // Set csi flags
    csi->vsync_pol = 1;
    csi->hsync_pol = 0;
    csi->pixck_pol = 1;
    csi->frame_sync = 0;
    csi->mono_bpp = 1;
    csi->rgb_swap = 0;
    csi->blc_size = 8;
    csi->jpg_format = 4;
    csi->yuv_format = SUBFORMAT_ID_YVU422;

    return 0;
}
#endif // (OMV_OV5640_ENABLE == 1)

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

    // 逐条复刻 Titan_Mini_Openmv/platform/camera/ov5640_mipi_regs.h：
    // 寄存器、值、写入顺序全部一致。顺序有意义 —— MIPI 使能 (0x300E/0x302E) 与
    // 时钟域/MIPI 时序 (0x3007/0x4800) 排在 PLL 与模拟前端之后；0x3035/0x3036
    // 由 reset() 末尾的 clocks[] 补写，与 Titan 相同。
    // {0xAA,0xAA,ms} 是延时标记，不写寄存器。

    // ========== 系统 / PLL / DVP 引脚 / 复位 ==========
    { 0x31, 0x03, 0x11 },
    { 0x30, 0x08, 0x82 },
    { 0xAA, 0xAA, 255 },  // 等待 255 ms
    { 0x30, 0x08, 0x42 },
    { 0x31, 0x03, 0x03 },
    { 0xAA, 0xAA, 5   },  // 等待 5 ms
    { 0x30, 0x17, 0x00 },
    { 0x30, 0x18, 0x00 },
    { 0x30, 0x34, 0x18 },
    { 0x30, 0x37, 0x13 },
    { 0x30, 0x2C, 0xC2 },
    { 0x31, 0x08, 0x01 },

    // ========== 模拟前端 ==========
    { 0x36, 0x30, 0x2E },
    { 0x36, 0x32, 0xE2 },
    { 0x36, 0x33, 0x23 },
    { 0x36, 0x21, 0xE0 },
    { 0x37, 0x04, 0xA0 },
    { 0x37, 0x03, 0x5A },
    { 0x37, 0x15, 0x78 },
    { 0x37, 0x17, 0x01 },
    { 0x37, 0x0B, 0x60 },
    { 0x37, 0x05, 0x1A },

    // ========== 模拟偏置 ==========
    { 0x39, 0x05, 0x02 },
    { 0x39, 0x06, 0x10 },
    { 0x39, 0x01, 0x0A },
    { 0x37, 0x31, 0x12 },
    { 0x36, 0x00, 0x08 },
    { 0x36, 0x01, 0x33 },
    { 0x30, 0x2D, 0x60 },
    { 0x36, 0x20, 0x52 },
    { 0x37, 0x1B, 0x20 },

    // ========== VFIFO / JPEG / 测试图案 ==========
    { 0x47, 0x1C, 0x50 },

    // ========== AEC / AGC（亮度目标、增益上限、频闪） ==========
    // 增益天花板对齐 OpenMV 上游（1.7.3 与 master 此处逐字节一致）：
    //   0x80 =  8.0x  (Titan 值，暗处不够用)
    //   0xF8 = 15.5x
    // 编码为 增益×16（与 0x350A real gain、get_gain_db 的 /16 同式）。
    { 0x3A, 0x18, 0x00 },
    { 0x3A, 0x19, 0xF8 },
    { 0x36, 0x35, 0x1C },
    { 0x36, 0x34, 0x40 },
    { 0x36, 0x22, 0x01 },

    // ========== 50/60Hz 检测 ==========
    { 0x3C, 0x04, 0x28 },
    { 0x3C, 0x05, 0x98 },
    { 0x3C, 0x06, 0x00 },
    { 0x3C, 0x07, 0x08 },
    { 0x3C, 0x08, 0x00 },
    { 0x3C, 0x09, 0x1C },
    { 0x3C, 0x0A, 0x9C },
    { 0x3C, 0x0B, 0x40 },

    // ========== 窗口 / 子采样 / 输出尺寸 / HTS-VTS ==========
    { 0x38, 0x20, 0x41 },
    { 0x38, 0x21, 0x01 },
    { 0x38, 0x00, 0x00 },
    { 0x38, 0x01, 0x20 },
    { 0x38, 0x02, 0x00 },
    { 0x38, 0x03, 0x16 },
    { 0x38, 0x04, 0x0A },
    { 0x38, 0x05, 0x20 },
    { 0x38, 0x06, 0x07 },
    { 0x38, 0x07, 0x96 },
    { 0x38, 0x10, 0x00 },
    { 0x38, 0x11, 0x10 },
    { 0x38, 0x12, 0x00 },
    { 0x38, 0x13, 0x10 },
    { 0x38, 0x08, 0x02 },
    { 0x38, 0x09, 0x80 },
    { 0x38, 0x0A, 0x01 },
    { 0x38, 0x0B, 0xE0 },
    { 0x38, 0x14, 0x31 },
    { 0x38, 0x15, 0x31 },
    { 0x37, 0x08, 0x64 },

    // ========== 黑电平校正 BLC ==========
    { 0x40, 0x01, 0x02 },
    { 0x40, 0x05, 0x1A },
    { 0x30, 0x00, 0x00 },
    { 0x30, 0x02, 0x1C },
    { 0x30, 0x04, 0xFF },
    { 0x30, 0x06, 0xC3 },
    { 0x30, 0x0E, 0x44 },  // 2-lane MIPI 使能
    { 0x30, 0x2E, 0x08 },

    // ========== 输出格式 ==========
    { 0x43, 0x00, 0x32 },

    // ========== ISP 使能 / 测试图案 ==========
    { 0x50, 0x1F, 0x00 },
    { 0x44, 0x07, 0x04 },
    // bit7 = Lenc（镜头阴影校正）。
    // 实测开（0xA7）比关（0x27）观感更好，保留开启 —— 与 Titan 一致。
    // 注：曾按「四角偏亮 + 色度饱和」判为过度补偿，但那组测量数据不可靠
    // （四角亮度达中心 151%，实际拍摄时四角采样窗内混入了非白区域）。
    // 观感结论优先于该推断。
    { 0x50, 0x00, 0xA7 },

    // ========== AWB 手动增益起点 ==========
    { 0x34, 0x06, 0x01 },
    { 0x34, 0x00, 0x06 },
    { 0x34, 0x01, 0x80 },
    { 0x34, 0x02, 0x04 },
    { 0x34, 0x03, 0x00 },
    { 0x34, 0x04, 0x06 },
    { 0x34, 0x05, 0x00 },

    // ========== AWB 系数表 ==========
    { 0x51, 0x80, 0xFF },
    { 0x51, 0x81, 0xF2 },
    { 0x51, 0x82, 0x00 },
    { 0x51, 0x83, 0x14 },
    { 0x51, 0x84, 0x25 },
    { 0x51, 0x85, 0x24 },
    { 0x51, 0x86, 0x16 },
    { 0x51, 0x87, 0x16 },
    { 0x51, 0x88, 0x16 },
    { 0x51, 0x89, 0x62 },
    { 0x51, 0x8A, 0x62 },
    { 0x51, 0x8B, 0xF0 },
    { 0x51, 0x8C, 0xB2 },
    { 0x51, 0x8D, 0x50 },
    { 0x51, 0x8E, 0x30 },
    { 0x51, 0x8F, 0x30 },
    { 0x51, 0x90, 0x50 },
    { 0x51, 0x91, 0xF8 },
    { 0x51, 0x92, 0x04 },
    { 0x51, 0x93, 0x70 },
    { 0x51, 0x94, 0xF0 },
    { 0x51, 0x95, 0xF0 },
    { 0x51, 0x96, 0x03 },
    { 0x51, 0x97, 0x01 },
    { 0x51, 0x98, 0x04 },
    { 0x51, 0x99, 0x12 },
    { 0x51, 0x9A, 0x04 },
    { 0x51, 0x9B, 0x00 },
    { 0x51, 0x9C, 0x06 },
    { 0x51, 0x9D, 0x82 },
    { 0x51, 0x9E, 0x38 },

    // ========== 色彩矩阵 ==========
    { 0x53, 0x81, 0x1E },
    { 0x53, 0x82, 0x5B },
    { 0x53, 0x83, 0x14 },
    { 0x53, 0x84, 0x06 },
    { 0x53, 0x85, 0x82 },
    { 0x53, 0x86, 0x88 },
    { 0x53, 0x87, 0x7C },
    { 0x53, 0x88, 0x60 },
    { 0x53, 0x89, 0x1C },
    { 0x53, 0x8A, 0x01 },
    { 0x53, 0x8B, 0x98 },

    // ========== ISP 锐度控制 ==========
    { 0x53, 0x00, 0x08 },
    { 0x53, 0x01, 0x30 },
    { 0x53, 0x02, 0x5F },
    { 0x53, 0x03, 0x10 },
    { 0x53, 0x04, 0x08 },
    { 0x53, 0x05, 0x30 },
    { 0x53, 0x06, 0x28 },
    { 0x53, 0x07, 0x38 },
    { 0x53, 0x09, 0x08 },
    { 0x53, 0x0A, 0x30 },
    { 0x53, 0x0B, 0x04 },
    { 0x53, 0x0C, 0x06 },

    // ========== Gamma ==========
    { 0x54, 0x80, 0x01 },
    { 0x54, 0x81, 0x06 },
    { 0x54, 0x82, 0x12 },
    { 0x54, 0x83, 0x24 },
    { 0x54, 0x84, 0x4A },
    { 0x54, 0x85, 0x58 },
    { 0x54, 0x86, 0x65 },
    { 0x54, 0x87, 0x72 },
    { 0x54, 0x88, 0x7D },
    { 0x54, 0x89, 0x88 },
    { 0x54, 0x8A, 0x92 },
    { 0x54, 0x8B, 0xA3 },
    { 0x54, 0x8C, 0xB2 },
    { 0x54, 0x8D, 0xC8 },
    { 0x54, 0x8E, 0xDD },
    { 0x54, 0x8F, 0xF0 },
    { 0x54, 0x90, 0x15 },

    // ========== UV 调整 / 亮度 / 对比度 ==========
    { 0x55, 0x80, 0x06 },
    { 0x55, 0x83, 0x40 },
    { 0x55, 0x84, 0x20 },
    { 0x55, 0x89, 0x10 },
    { 0x55, 0x8A, 0x00 },
    { 0x55, 0x8B, 0xF8 },
    { 0x55, 0x85, 0x00 },
    { 0x55, 0x86, 0x20 },
    { 0x55, 0x87, 0x00 },
    { 0x55, 0x88, 0x01 },
    { 0x50, 0x1D, 0x40 },
    // 第二次写 0x5000，与前面 ISP 使能段保持一致（Lenc 开）。
    { 0x50, 0x00, 0xA7 },

    // ========== 镜头阴影校正 (Lens) ==========
    { 0x58, 0x00, 0x20 },
    { 0x58, 0x01, 0x19 },
    { 0x58, 0x02, 0x17 },
    { 0x58, 0x03, 0x16 },
    { 0x58, 0x04, 0x18 },
    { 0x58, 0x05, 0x21 },
    { 0x58, 0x06, 0x0F },
    { 0x58, 0x07, 0x0A },
    { 0x58, 0x08, 0x07 },
    { 0x58, 0x09, 0x07 },
    { 0x58, 0x0A, 0x0A },
    { 0x58, 0x0B, 0x0C },
    { 0x58, 0x0C, 0x0A },
    { 0x58, 0x0D, 0x03 },
    { 0x58, 0x0E, 0x01 },
    { 0x58, 0x0F, 0x01 },
    { 0x58, 0x10, 0x03 },
    { 0x58, 0x11, 0x09 },
    { 0x58, 0x12, 0x0A },
    { 0x58, 0x13, 0x03 },
    { 0x58, 0x14, 0x01 },
    { 0x58, 0x15, 0x01 },
    { 0x58, 0x16, 0x03 },
    { 0x58, 0x17, 0x08 },
    { 0x58, 0x18, 0x10 },
    { 0x58, 0x19, 0x0A },
    { 0x58, 0x1A, 0x06 },
    { 0x58, 0x1B, 0x06 },
    { 0x58, 0x1C, 0x08 },
    { 0x58, 0x1D, 0x0E },
    { 0x58, 0x1E, 0x22 },
    { 0x58, 0x1F, 0x18 },
    { 0x58, 0x20, 0x13 },
    { 0x58, 0x21, 0x12 },
    { 0x58, 0x22, 0x16 },
    { 0x58, 0x23, 0x1E },
    { 0x58, 0x24, 0x64 },
    { 0x58, 0x25, 0x2A },
    { 0x58, 0x26, 0x2C },
    { 0x58, 0x27, 0x2A },
    { 0x58, 0x28, 0x46 },
    { 0x58, 0x29, 0x2A },
    { 0x58, 0x2A, 0x26 },
    { 0x58, 0x2B, 0x24 },
    { 0x58, 0x2C, 0x26 },
    { 0x58, 0x2D, 0x2A },
    { 0x58, 0x2E, 0x28 },
    { 0x58, 0x2F, 0x42 },
    { 0x58, 0x30, 0x40 },
    { 0x58, 0x31, 0x42 },
    { 0x58, 0x32, 0x08 },
    { 0x58, 0x33, 0x28 },
    { 0x58, 0x34, 0x26 },
    { 0x58, 0x35, 0x24 },
    { 0x58, 0x36, 0x26 },
    { 0x58, 0x37, 0x2A },
    { 0x58, 0x38, 0x44 },
    { 0x58, 0x39, 0x4A },
    { 0x58, 0x3A, 0x2C },
    { 0x58, 0x3B, 0x2A },
    { 0x58, 0x3C, 0x46 },
    { 0x58, 0x3D, 0xCE },

    // ========== AWB 统计权重 (AVG) ==========
    { 0x56, 0x88, 0x22 },
    { 0x56, 0x89, 0x22 },
    { 0x56, 0x8A, 0x42 },
    { 0x56, 0x8B, 0x24 },
    { 0x56, 0x8C, 0x42 },
    { 0x56, 0x8D, 0x24 },
    { 0x56, 0x8E, 0x22 },
    { 0x56, 0x8F, 0x22 },
    { 0x50, 0x25, 0x00 },
    // ========== AEC 亮度目标 + 曝光上下限 ==========
    // 六件套对齐 OpenMV 上游（1.7.3 与 master 此处逐字节一致）。
    // 原来是 Titan 的 40/30/40/30/71/20，比上游低约 1 EV，暗处偏暗。
    // 这六个必须一起改：只抬目标而不同步抬上限，AEC 要不到光就会把增益顶到
    // ceiling，噪声最大、AWB 在脏数据上收敛，表现为每次运行偏色还不一样。
    //   0x3a0f/10  高光目标      42/38  (原 40/30)
    //   0x3a1b/1e  低光目标      44/36  (原 40/30)
    //   0x3a11     曝光上限      60     (原 71)
    //   0x3a1f     曝光下限      10     (原 20)
    { 0x3A, 0x0F, 0x42 },
    { 0x3A, 0x10, 0x38 },
    { 0x3A, 0x1B, 0x44 },
    { 0x3A, 0x1E, 0x36 },
    { 0x3A, 0x11, 0x60 },
    { 0x3A, 0x1F, 0x10 },

    // ========== MIPI 控制 / 时序 ==========
    { 0x48, 0x00, 0x24 },
    { 0x30, 0x07, 0xFB },  // MIPI 时钟域使能
    { 0x38, 0x0C, 0x08 },  // HTS=2128
    { 0x38, 0x0D, 0x50 },
    { 0x38, 0x0E, 0x04 },  // VTS=1200
    { 0x38, 0x0F, 0xB0 },
    { 0x3C, 0x01, 0xB4 },
    { 0x3C, 0x00, 0x04 },
    { 0x3A, 0x08, 0x00 },
    { 0x3A, 0x09, 0x93 },
    { 0x3A, 0x0E, 0x06 },
    { 0x3A, 0x0A, 0x00 },
    { 0x3A, 0x0B, 0x7B },
    { 0x3A, 0x0D, 0x08 },
    { 0x3A, 0x00, 0x3C },  // bit2=夜视：允许曝光越过帧率上限，暗光临界时诱发 AWB 抖动
    { 0x3A, 0x02, 0x05 },
    { 0x3A, 0x03, 0x44 },
    { 0x3A, 0x14, 0x05 },
    { 0x3A, 0x15, 0x44 },
    { 0x36, 0x18, 0x00 },
    { 0x36, 0x12, 0x29 },
    { 0x37, 0x08, 0x64 },
    { 0x37, 0x09, 0x52 },
    { 0x37, 0x0C, 0x03 },
    { 0x40, 0x01, 0x02 },
    { 0x40, 0x04, 0x02 },
    { 0x40, 0x05, 0x1A },
    { 0x47, 0x13, 0x03 },
    { 0x46, 0x0B, 0x35 },
    { 0x46, 0x0C, 0x22 },
    { 0x48, 0x37, 0x0A },
    { 0x38, 0x24, 0x01 },
    { 0x50, 0x01, 0xA3 },
    { 0x35, 0x03, 0x00 },  // AEC+AGC 全自动（Titan 依赖复位默认值，这里显式写同值）
    { 0x30, 0x08, 0x02 },
    { 0xAA, 0xAA, 5   },  // 等待 5 ms
    { 0x50, 0x3D, 0x00 },
    { 0x47, 0x41, 0x00 },

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

    // 复位/上电时序与全部配置都在 default_regs 里，含 Titan 的两段
    // REQUEST_SOFTWARE_WAIT（软复位后 255ms、唤醒后 5ms）。写成表内延时标记
    // 而不是在这里硬编码，是为了让整条序列与 Titan 的表格逐条对应、便于比对。
    for (int i = 0; default_regs[i][0]; i++) {
        int addr = (default_regs[i][0] << 8) | (default_regs[i][1] << 0);
        int data = default_regs[i][2];

        if (addr == 0xAAAA) {
            // 延时标记：data 是毫秒数，不写寄存器。
            mp_hal_delay_ms(data);
            continue;
        }

        ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, addr, 2, data, 1);
    }

    // 表写完后 Titan 还会补写这一组（titan_camera.c:76-84 的 clocks[]），
    // 顺序与值都照搬：PLL 分频放在最后、AWB 转自动、停流出、方向位归位。
    static const uint8_t clocks[][3] = {
        { 0x30, 0x35, 0x12 },   // PLL: sys_div=1, mipi_div=2
        { 0x30, 0x36, 0x8C },   // PLL: multiplier=140
        { 0x30, 0x37, 0x13 },   // PLL: root_div=2, pre_div=3
        { 0x31, 0x08, 0x01 },   // 系统根分频
        { 0x34, 0x06, 0x00 },   // AWB 转自动
        { 0x42, 0x02, 0x0F },   // MIPI FIFO 复位 / 停流，等 CaptureStart 后再放行
        { 0x38, 0x20, 0x41 },   // sensor vflip + ISP vflip
        { 0x38, 0x21, 0x01 },   // sensor mirror
    };
    for (size_t i = 0; i < sizeof(clocks) / sizeof(clocks[0]); i++) {
        ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr,
                                 (clocks[i][0] << 8) | clocks[i][1], 2, clocks[i][2], 1);
        // 停流之后等 5ms，与 Titan 在 sensor_reset 末尾的 mdelay(5) 对应
        if (clocks[i][0] == 0x42 && clocks[i][1] == 0x02) {
            mp_hal_delay_ms(5);
        }
    }

    // MIPI 虚拟通道：只改 [7:6]，其余控制位保留（Titan titan_camera.c:88-98）。
    // 原先直写 0x00 会把低 6 位一并清零，两边的最终值可能不同。
    {
        uint8_t mipi_ctrl = 0;
        if (omv_i2c_read_reg(csi->i2c, csi->slv_addr, 0x4814, 2, &mipi_ctrl, 1) == 0) {
            ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, 0x4814, 2,
                                     mipi_ctrl & 0x3F, 1);
        } else {
            // 读失败就退回原先的直写 0（VC=0，低 6 位也清零）
            ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, 0x4814, 2, 0x00, 1);
        }
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
    // 与 Titan 一致: 只改输出尺寸 DVPHO/DVPVO (0x3808-0x380B)。
    // 读取窗口 0x3800-0x3807 (X 32~2592 / Y 22~1942)、预缩放偏移 0x3810-0x3813、
    // 子采样增量 0x3814/0x3815 = 0x31/0x31、HTS/VTS = 2128/1200 在两档之间**完全相同**
    // (上游 openmv/drivers/sensors/ov5640.c 的 set_framesize 逐行代入也是这个结果:
    //  readout_w/h 恒为最大值, sensor_div 两档都是 2, 窗口/offset/inc/时序全同)。
    // 缩小由传感器 ISP 缩放器完成: VGA 是 2:1, QVGA 是 4:1。
    // VIN 侧的接收几何由 ports/ra8/omv_csi.c 的 omv_vin_apply_geometry() 跟随。
    if (framesize != OMV_CSI_FRAMESIZE_VGA && framesize != OMV_CSI_FRAMESIZE_QVGA) {
        return -1;
    }

    uint16_t w = csi->resolution[framesize][0];
    uint16_t h = csi->resolution[framesize][1];
    int ret = 0;

    ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, TIMING_DVPHO_H, 2, w >> 8, 1);
    ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, TIMING_DVPHO_L, 2, w, 1);
    ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, TIMING_DVPVO_H, 2, h >> 8, 1);
    ret |= omv_i2c_write_reg(csi->i2c, csi->slv_addr, TIMING_DVPVO_L, 2, h, 1);

    return ret;
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

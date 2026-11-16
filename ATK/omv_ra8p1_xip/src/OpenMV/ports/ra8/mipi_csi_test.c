/**
 * MIPI CSI callback test - titan flow + OMV-style polarity sweep for reset.
 *
 * Steps follow titan_mini_display_camera_mipi_csi's camera_init():
 *   1. Configure GPIOs (reset=P11_0, power=P7_10)
 *   2. Start XCLK (GPT)
 *   3. Init I2C
 *   4. Hardware reset -- swept over 4 power/reset polarities (OMV style)
 *   5. Read product ID to find working polarity
 *   6. Write MIPI sensor registers
 *   7. Configure PLL
 *   8. Set MIPI virtual channel
 *   9. Stream off
 *   10. R_VIN_Open
 *   11. Stream on
 *   12. R_VIN_CaptureStart
 *   13. Wait for VIN callback
 */
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "hal_data.h"
#include "omv_csi.h"
#include "omv_gpio.h"
#include "omv_i2c.h"
#include "omv_boardconfig.h"

// --- Test state ---
static volatile int g_test_vin_frame_count = 0;
static volatile uint8_t *g_test_vin_last_buffer = NULL;

//// --- VIN callback (should be called by FSP on frame complete) ---
//void cam_vin_callback(capture_callback_args_t *p_args) {
//    vin_interrupt_status_t interrupt_status = (vin_interrupt_status_t) p_args->interrupt_status;
//    switch (p_args->event) {
//        case VIN_EVENT_NOTIFY:
//            if (interrupt_status.bits.frame_complete) {
//                g_test_vin_frame_count++;
//                g_test_vin_last_buffer = p_args->p_buffer;
//            }
//            break;
//        case VIN_EVENT_ERROR:
//            printf("[MIPI_TEST] VIN ERROR!\n");
//            break;
//        default:
//            break;
//    }
//}
//
//// MIPI CSI callback (stub)
//void cam_mipi_csi_callback(mipi_csi_callback_args_t *p_args) {
//    (void)p_args;
//}

// --- I2C helpers ---
// Reference omv_i2c.c: every transfer must set the slave address first
// via R_IIC_MASTER_SlaveAddressSet, then wait for the callback flag.
// OMV's OV5640_SLV_ADDR = 0x78 (8-bit). R_IIC_MASTER needs the 7-bit form,
// which is 0x78 >> 1 = 0x3C. This board's OV5640 SID pin is high.
#define TEST_SLV_ADDR_8BIT   (0x78)   // OMV 8-bit slave address (matches omv_csi.h)
#define TEST_SLV_ADDR_7BIT   (0x3C)   // 0x78 >> 1, passed to R_IIC_MASTER

extern volatile uint8_t omv_i2c_rx_status;
extern volatile uint8_t omv_i2c_tx_status;

// Wait for I2C transfer complete (poll callback flag with timeout)
static int test_i2c_wait(volatile uint8_t *flag, const char *what) {
    uint32_t ts = mp_hal_ticks_ms();
    while (*flag == 0) {
        if (mp_hal_ticks_ms() - ts > 1000) {
            R_IIC_MASTER_Abort(&OMV_I2C_HANDLE);
            *flag = 0;
            printf("[I2C] %s timeout\n", what);
            return -1;
        }
    }
    *flag = 0;
    return 0;
}

// Set slave address (7-bit) before every transfer
static int test_i2c_set_addr(void) {
    fsp_err_t err = R_IIC_MASTER_SlaveAddressSet(&OMV_I2C_HANDLE, TEST_SLV_ADDR_7BIT, I2C_MASTER_ADDR_MODE_7BIT);
    if (err != FSP_SUCCESS) {
        printf("[I2C] SlaveAddressSet error %d\n", err);
        return -1;
    }
    return 0;
}

// titan-style read: write 2-byte reg addr (no stop), then restart + read 1 byte
static int test_i2c_read_reg(uint16_t addr, uint8_t *val) {
    uint8_t reg_addr[2] = { (uint8_t)(addr >> 8), (uint8_t)addr };

    // Write register address (no stop, so the read can restart on the same bus)
    if (test_i2c_set_addr() != 0) return -1;
    omv_i2c_tx_status = 0;
    fsp_err_t err = R_IIC_MASTER_Write(&OMV_I2C_HANDLE, reg_addr, 2, true); // nostop=true
    if (err != FSP_SUCCESS) { printf("[I2C] Write err %d\n", err); return -1; }
    if (test_i2c_wait(&omv_i2c_tx_status, "write") != 0) return -1;

    // Restart: set address again, then read 1 byte
    if (test_i2c_set_addr() != 0) return -1;
    omv_i2c_rx_status = 0;
    err = R_IIC_MASTER_Read(&OMV_I2C_HANDLE, val, 1, false); // stop=true
    if (err != FSP_SUCCESS) { printf("[I2C] Read err %d\n", err); return -1; }
    if (test_i2c_wait(&omv_i2c_rx_status, "read") != 0) return -1;

    return 0;
}

// titan-style write: write 2-byte reg addr + 1-byte data
static int test_i2c_write_reg(uint16_t addr, uint8_t val) {
    uint8_t data[3] = { (uint8_t)(addr >> 8), (uint8_t)addr, val };

    if (test_i2c_set_addr() != 0) return -1;
    omv_i2c_tx_status = 0;
    fsp_err_t err = R_IIC_MASTER_Write(&OMV_I2C_HANDLE, data, 3, false); // stop=true
    if (err != FSP_SUCCESS) { printf("[I2C] Write err %d\n", err); return -1; }
    if (test_i2c_wait(&omv_i2c_tx_status, "write") != 0) return -1;

    return 0;
}

static int test_i2c_write_array(const uint8_t regs[][3]) {
    int ret = 0;
    for (int i = 0; regs[i][0]; i++) {
        uint16_t addr = (regs[i][0] << 8) | regs[i][1];
        ret |= test_i2c_write_reg(addr, regs[i][2]);
    }
    return ret;
}

// --- OV5640 MIPI register table (MINIMAL set for MIPI bring-up) ---
// Only the registers REQUIRED to make the sensor output MIPI 2-lane YUV422
// are active. Everything else (image quality / frame rate tuning) is kept
// below inside #if 0 -- commented but not deleted. To re-enable a group,
// cut the #if 0 / #endif and move the entries above the {0x00,0x00,0x00} end.
static const uint8_t mipi_init_regs[][3] = {

    // ========== ESSENTIAL: reset / power sequence ==========
    { 0x31, 0x03, 0x11 }, // SCCB system control: software reset via SCCB
    { 0x30, 0x08, 0x82 }, // software reset (bit7=1, self-clears)
    { 0x30, 0x08, 0x42 }, // power down (bit6=1) + MIPI mode
    { 0x31, 0x03, 0x03 }, // SCCB back to normal mode

    // ========== ESSENTIAL: DVP pins as input ==========
    // If the DVP outputs stay enabled they can drive PCLK and block the
    // MIPI PHY from ever locking. These two are the classic missing piece.
    { 0x30, 0x17, 0x00 }, // DVP: Frex/Vsync/Href/PCLK/D[9:6] -> INPUT
    { 0x30, 0x18, 0x00 }, // DVP: D[5:0]/GPIO[1:0] -> INPUT
    { 0x30, 0x2c, 0xc2 }, // bit1 FREX enable, bit6-7 output drive

    // ========== ESSENTIAL: PLL / PHY mode ==========
    { 0x30, 0x34, 0x18 }, // PLL control0: MIPI 8-bit mode, PCLK root div=8

    // (0x3000-0x3006 system clock/reset ① moved to 试排除 block below)

    // ========== ESSENTIAL: MIPI interface enable ==========
    { 0x30, 0x0e, 0x44 }, // MIPI control: 2-lane + MIPI enable   <-- KEY
    { 0x30, 0x2e, 0x08 }, // system control 1 (titan writes it right after 0x300E)
    { 0x48, 0x00, 0x24 }, // MIPI Control 00 (per-lane clock setup)
    { 0x48, 0x37, 0x0a }, // MIPI global timing
    { 0x30, 0x07, 0xfb }, // Disable DVP PCLK, enable MIPI clock domain  <-- KEY

    // (0x3824 MIPI timing tweak ② moved to 试排除 block below)

    // ========== ESSENTIAL: output format ==========
    // Must match VIN input_format=YCBCR422_8_BIT / data_type=YUV422_8_BIT.
    { 0x43, 0x00, 0x32 }, // format: YUV422 8-bit, YUYV order
    { 0x50, 0x1f, 0x00 }, // format MUX: ISP YUV422 output

    // (0x4713/0x460B/0x460C output data path ③ moved to 试排除 block below)

    // ========== ESSENTIAL: ISP on (produces valid YUV) ==========
    { 0x50, 0x00, 0xa7 }, // ISP: Lenc/gamma/BPC/WPC/CIP enable
    { 0x50, 0x01, 0xa3 }, // ISP: AWB/color matrix/UV/scale/SDE enable

    // ========== ESSENTIAL: output size 320x240 (QVGA) ==========
    // 与 OMV set_framesize(QVGA) 的寄存器路径一致: 窗口 2624x1948 + div2
    // (0x3814/0x3815=0x31) 得到 ~1312x974, ISP 缩放器自动缩到 0x3808/0x380a。
    // 注意: g_cam_vin_cfg 的 VIN preclip 仍是 639/479(640x480)。
    // 若 callback 仍触发 -> preclip 不挡 frame_complete(只裁剪);
    // 若不通 -> 下一步把 test 的 VIN preclip 也改成 319/239 再试。
    { 0x38, 0x08, 0x01 }, { 0x38, 0x09, 0x40 }, // Xout = 0x0140 = 320
    { 0x38, 0x0a, 0x00 }, { 0x38, 0x0b, 0xf0 }, // Yout = 0x00F0 = 240

    // (0x3800-0x3807 / 0x3810-0x3815 input window + steps ④ moved to 试排除 block)

    // ========== ESSENTIAL: frame timing (HTS/VTS) ==========
    // Sets the line/frame length the MIPI output is generated against.
    // If defaults are unusual, the transmitted frame size/timing can't match
    // VIN preclip or the frame rate can drop below the 5s test window.
    { 0x38, 0x0c, 0x08 }, { 0x38, 0x0d, 0x50 }, // HTS = 0x0850 = 2128
    { 0x38, 0x0e, 0x04 }, { 0x38, 0x0f, 0xb0 }, // VTS = 0x04B0 = 1200

    // ========== ESSENTIAL: mirror / flip (orientation) ==========
    { 0x38, 0x20, 0x41 }, // sensor vflip + ISP vflip
    { 0x38, 0x21, 0x01 }, // sensor mirror

    // ========== ESSENTIAL: wake up ==========
    { 0x30, 0x08, 0x02 }, // wake up (clear power-down bit6)

    // ========== ESSENTIAL: disable test patterns ==========
    { 0x50, 0x3d, 0x00 }, // ISP test pattern off
    { 0x47, 0x41, 0x00 }, // test pattern color bar off

    // ================================================================
    // 【试排除 - 调试中加的 ①②③④】
    // 这些是排查过程中逐个加上去的，但单独加时都没能让 callback 通；
    // 真正让数据流通的是 HTS/VTS + 0x302E。现在把它们全部排除，
    // 验证它们是否纯属多余。若本版仍能进 callback -> 它们可删；
    // 若进不了 -> 把下面某组放回有效区逐个找回。
    // ================================================================

    // ① system clock / reset enables (0x3000-0x3006)
    { 0x30, 0x00, 0x00 }, // system reset control 0
    { 0x30, 0x02, 0x1c }, // system reset control 2
    { 0x30, 0x04, 0xff }, // clock enable 0
    { 0x30, 0x06, 0xc3 }, // clock enable 2
    // ② MIPI timing tweak ("add by bright")
    { 0x38, 0x24, 0x01 }, // MIPI timing tweak

    // ③ output data path select
    { 0x47, 0x13, 0x03 }, // JPEG mode select
    { 0x46, 0x0b, 0x35 }, // VFIFO
    { 0x46, 0x0c, 0x22 }, // VFIFO PCLK manual
    // ④ input window + scaling steps
    { 0x38, 0x00, 0x00 }, { 0x38, 0x01, 0x00 }, // Xstart = 0
    { 0x38, 0x02, 0x00 }, { 0x38, 0x03, 0x00 }, // Ystart = 0
    { 0x38, 0x04, 0x0a }, { 0x38, 0x05, 0x3f }, // Xend = 2623
    { 0x38, 0x06, 0x07 }, { 0x38, 0x07, 0x9b }, // Yend = 1947
    { 0x38, 0x10, 0x00 }, { 0x38, 0x11, 0x10 }, // pre-scale X offset
    { 0x38, 0x12, 0x00 }, { 0x38, 0x13, 0x06 }, // pre-scale Y offset
    { 0x38, 0x14, 0x31 }, // horizontal subsample increment
    { 0x38, 0x15, 0x31 }, // vertical subsample increment
#if 0


#endif

    // ================================================================
    // The registers below are NOT required for basic MIPI output --
    // they only tune image quality (AWB/color/gamma/lens) or frame rate
    // (HTS/VTS). Kept for reference; commented out so the minimal set
    // above can be tested first.
    // ================================================================
#if 0
    // Analog / ADC front-end (does not affect MIPI data path)
    { 0x36, 0x30, 0x2e }, // analog control
    { 0x36, 0x32, 0xe2 }, // analog control
    { 0x36, 0x33, 0x23 }, // analog control
    { 0x36, 0x21, 0xe0 }, // analog control
    { 0x37, 0x04, 0xa0 }, // sensor control
    { 0x37, 0x03, 0x5a }, // sensor control
    { 0x37, 0x15, 0x78 }, // sensor control
    { 0x37, 0x17, 0x01 }, // sensor control
    { 0x37, 0x0b, 0x60 }, // sensor control
    { 0x37, 0x05, 0x1a }, // sensor control
    { 0x39, 0x05, 0x02 }, // sensor control
    { 0x39, 0x06, 0x10 }, // sensor control
    { 0x39, 0x01, 0x0a }, // sensor control
    { 0x37, 0x31, 0x12 }, // sensor control
    { 0x36, 0x00, 0x08 }, // sensor control
    { 0x36, 0x01, 0x33 }, // sensor control
    { 0x30, 0x2d, 0x60 }, // sensor control
    { 0x36, 0x20, 0x52 }, // sensor control
    { 0x37, 0x1b, 0x20 }, // sensor control
    { 0x47, 0x1c, 0x50 }, // sensor control

    // AEC / gain ceiling
    { 0x3a, 0x18, 0x00 }, // gain ceiling H
    { 0x3a, 0x19, 0x80 }, // gain ceiling L (titan: 0x80)

    // Analog control
    { 0x36, 0x35, 0x1c }, // analog control
    { 0x36, 0x34, 0x40 }, // analog control
    { 0x36, 0x22, 0x01 }, // analog control

    // 50/60 Hz detection thresholds
    { 0x3c, 0x04, 0x28 }, // AEC threshold low sum
    { 0x3c, 0x05, 0x98 }, // AEC threshold high sum
    { 0x3c, 0x06, 0x00 }, // lightmeter1 threshold H
    { 0x3c, 0x07, 0x08 }, // lightmeter1 threshold L
    { 0x3c, 0x08, 0x00 }, // lightmeter2 threshold H
    { 0x3c, 0x09, 0x1c }, // lightmeter2 threshold L
    { 0x3c, 0x0a, 0x9c }, // sample number H
    { 0x3c, 0x0b, 0x40 }, // sample number L

    // (0x3800-0x3807 window / 0x3810-0x3815 offset+steps moved above --
    //  always active, see ESSENTIAL section)

    // Sensor timing
    { 0x37, 0x08, 0x64 }, // sensor timing
    { 0x40, 0x01, 0x02 }, // BLC start line
    { 0x40, 0x05, 0x1a }, // BLC always update

    // (0x3000/0x3002/0x3004/0x3006 system clock/reset + 0x3824 moved
    //  above the #if 0 -- they are now always active, see ESSENTIAL section)

    // System control (0x302E moved above -- always active)
    { 0x44, 0x07, 0x04 }, // JPEG QS (not used for MIPI YUV)

    // AWB manual gains
    { 0x34, 0x06, 0x01 }, // AWB gain manual enable
    { 0x34, 0x00, 0x06 }, { 0x34, 0x01, 0x80 }, // R gain
    { 0x34, 0x02, 0x04 }, { 0x34, 0x03, 0x00 }, // G gain
    { 0x34, 0x04, 0x06 }, { 0x34, 0x05, 0x00 }, // B gain

    // AWB coefficient table
    { 0x51, 0x80, 0xff }, { 0x51, 0x81, 0xf2 }, { 0x51, 0x82, 0x00 }, { 0x51, 0x83, 0x14 },
    { 0x51, 0x84, 0x25 }, { 0x51, 0x85, 0x24 }, { 0x51, 0x86, 0x16 }, { 0x51, 0x87, 0x16 },
    { 0x51, 0x88, 0x16 }, { 0x51, 0x89, 0x62 }, { 0x51, 0x8a, 0x62 }, { 0x51, 0x8b, 0xf0 },
    { 0x51, 0x8c, 0xb2 }, { 0x51, 0x8d, 0x50 }, { 0x51, 0x8e, 0x30 }, { 0x51, 0x8f, 0x30 },
    { 0x51, 0x90, 0x50 }, { 0x51, 0x91, 0xf8 }, { 0x51, 0x92, 0x04 }, { 0x51, 0x93, 0x70 },
    { 0x51, 0x94, 0xf0 }, { 0x51, 0x95, 0xf0 }, { 0x51, 0x96, 0x03 }, { 0x51, 0x97, 0x01 },
    { 0x51, 0x98, 0x04 }, { 0x51, 0x99, 0x12 }, { 0x51, 0x9a, 0x04 }, { 0x51, 0x9b, 0x00 },
    { 0x51, 0x9c, 0x06 }, { 0x51, 0x9d, 0x82 }, { 0x51, 0x9e, 0x38 },

    // Color matrix
    { 0x53, 0x81, 0x1e }, { 0x53, 0x82, 0x5b }, { 0x53, 0x83, 0x14 }, { 0x53, 0x84, 0x06 },
    { 0x53, 0x85, 0x82 }, { 0x53, 0x86, 0x88 }, { 0x53, 0x87, 0x7c }, { 0x53, 0x88, 0x60 },
    { 0x53, 0x89, 0x1c }, { 0x53, 0x8a, 0x01 }, { 0x53, 0x8b, 0x98 },

    // Sharpness / Noise
    { 0x53, 0x00, 0x08 }, { 0x53, 0x01, 0x30 }, { 0x53, 0x02, 0x5f }, { 0x53, 0x03, 0x10 },
    { 0x53, 0x04, 0x08 }, { 0x53, 0x05, 0x30 }, { 0x53, 0x06, 0x28 }, { 0x53, 0x07, 0x38 },
    { 0x53, 0x09, 0x08 }, { 0x53, 0x0a, 0x30 }, { 0x53, 0x0b, 0x04 }, { 0x53, 0x0c, 0x06 },

    // Gamma table
    { 0x54, 0x80, 0x01 }, { 0x54, 0x81, 0x06 }, { 0x54, 0x82, 0x12 }, { 0x54, 0x83, 0x24 },
    { 0x54, 0x84, 0x4a }, { 0x54, 0x85, 0x58 }, { 0x54, 0x86, 0x65 }, { 0x54, 0x87, 0x72 },
    { 0x54, 0x88, 0x7d }, { 0x54, 0x89, 0x88 }, { 0x54, 0x8a, 0x92 }, { 0x54, 0x8b, 0xa3 },
    { 0x54, 0x8c, 0xb2 }, { 0x54, 0x8d, 0xc8 }, { 0x54, 0x8e, 0xdd }, { 0x54, 0x8f, 0xf0 },
    { 0x54, 0x90, 0x15 },

    // UV adjust / brightness / contrast / saturation
    { 0x55, 0x80, 0x06 }, { 0x55, 0x83, 0x40 }, { 0x55, 0x84, 0x20 }, { 0x55, 0x89, 0x10 },
    { 0x55, 0x8a, 0x00 }, { 0x55, 0x8b, 0xf8 },
    { 0x55, 0x85, 0x00 }, { 0x55, 0x86, 0x00 }, { 0x55, 0x87, 0x00 }, { 0x55, 0x88, 0x00 },
    { 0x50, 0x1d, 0x00 },

    // Lens shading correction table
    { 0x50, 0x00, 0xa7 }, { 0x58, 0x00, 0x20 }, { 0x58, 0x01, 0x19 }, { 0x58, 0x02, 0x17 },
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

    // AVG (average) table
    { 0x56, 0x88, 0x22 }, { 0x56, 0x89, 0x22 }, { 0x56, 0x8a, 0x42 }, { 0x56, 0x8b, 0x24 },
    { 0x56, 0x8c, 0x42 }, { 0x56, 0x8d, 0x24 }, { 0x56, 0x8e, 0x22 }, { 0x56, 0x8f, 0x22 },
    { 0x50, 0x25, 0x00 }, // AVG

    // AEC target
    { 0x3a, 0x0f, 0x40 }, { 0x3a, 0x10, 0x30 }, { 0x3a, 0x1b, 0x40 }, { 0x3a, 0x1e, 0x30 },
    { 0x3a, 0x11, 0x71 }, { 0x3a, 0x1f, 0x20 },

    // (0x380C-0x380F HTS/VTS moved above -- always active, see ESSENTIAL)

    // 50/60Hz detector
    { 0x3c, 0x01, 0xb4 }, { 0x3c, 0x00, 0x04 },
    { 0x3a, 0x08, 0x00 }, { 0x3a, 0x09, 0x93 },
    { 0x3a, 0x0e, 0x06 }, { 0x3a, 0x0a, 0x00 }, { 0x3a, 0x0b, 0x7b }, { 0x3a, 0x0d, 0x08 },

    // AEC/AGC power down domain control
    { 0x3a, 0x00, 0x3c }, { 0x3a, 0x02, 0x05 }, { 0x3a, 0x03, 0x44 }, { 0x3a, 0x14, 0x05 },
    { 0x3a, 0x15, 0x44 },

    // Misc
    { 0x36, 0x18, 0x00 }, { 0x36, 0x12, 0x29 }, { 0x37, 0x08, 0x64 }, { 0x37, 0x09, 0x52 },
    { 0x37, 0x0c, 0x03 },

    // BLC
    { 0x40, 0x01, 0x02 }, { 0x40, 0x04, 0x02 }, { 0x40, 0x05, 0x1a },

    // (0x4713/0x460B/0x460C moved above -- always active, see ESSENTIAL)
    // MIPI timing tweak 0x3824=0x01 moved above -- always active
#endif

    { 0x00, 0x00, 0x00 }  // end
};

// --- PLL configuration (from titan ov5640_configure_clocks) ---
static void test_ov5640_configure_clocks(void) {
    test_i2c_write_reg(0x3035, (1 << 4) | 2);       // sys_div=1, mipi_div=2
    test_i2c_write_reg(0x3036, 140);                 // multiplier=140
    test_i2c_write_reg(0x3037, (1 << 4) | 3);        // root_div=2, pre_div=3
    uint8_t lut[] = {0xF, 0, 1, 0xF, 2, 0xF, 0xF, 0xF, 3};
    test_i2c_write_reg(0x3108, (lut[1] << 4) | (lut[1] << 2) | lut[2]); // pclk=1,sclk2x=1,sclk=2
}

// --- Set MIPI virtual channel ---
static void test_ov5640_set_mipi_vc(uint32_t vc) {
    uint8_t tmp;
    test_i2c_read_reg(0x4814, &tmp);
    tmp = (tmp & ~(3 << 6)) | (vc << 6);
    test_i2c_write_reg(0x4814, tmp);
}

// --- Stream on/off ---
static void test_ov5640_stream_off(void) { test_i2c_write_reg(0x4202, 0x0F); }
static void test_ov5640_stream_on(void)  { test_i2c_write_reg(0x4202, 0x00); }

// --- Main test entry ---
void mipi_csi_test(void) {
    fsp_err_t err;
    uint8_t pid_h = 0, pid_l = 0;
    printf("\n===== MIPI CSI Test (titan flow + OMV reset sweep) =====\n");

    // ===== Step 1: Configure CSI GPIOs =====
    omv_gpio_config(OMV_CSI_RESET_PIN, OMV_GPIO_MODE_OUTPUT, OMV_GPIO_PULL_NONE, OMV_GPIO_SPEED_LOW, -1);
    omv_gpio_config(OMV_CSI_POWER_PIN, OMV_GPIO_MODE_OUTPUT, OMV_GPIO_PULL_NONE, OMV_GPIO_SPEED_LOW, -1);
    printf("[1] CSI GPIOs configured (reset=P11_0, power=P7_10)\n");

    // ===== Step 2: Start XCLK (GPT) =====
    err = R_GPT_Open(&OMV_GPT_TIM_HANDLE, &OMV_GPT_TIM_CFG);
    err |= R_GPT_Enable(&OMV_GPT_TIM_HANDLE);
    err |= R_GPT_Start(&OMV_GPT_TIM_HANDLE);
    printf("[2] XCLK GPT: %d\n", err);
    R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MILLISECONDS);

    // ===== Step 3: Init I2C (open IIC master, same handle as OMV) =====
    // Copy the FSP config into a runtime copy so we can tweak speed.
    i2c_master_cfg_t i2c_cfg = OMV_I2C_CFG;
    fsp_err_t ierr = R_IIC_MASTER_Open(&OMV_I2C_HANDLE, &i2c_cfg);
    printf("[3] R_IIC_MASTER_Open: %d\n", ierr);
    if (ierr != FSP_SUCCESS) {
        printf("[3] FAILED: I2C open error\n");
        return;
    }
    printf("[3] I2C initialized (slave addr will be set per transfer)\n");

    // ===== Step 4: Hardware reset with polarity sweep (OMV style) =====
    // Sweep 4 power/reset polarities until OV5640 responds.
    int detected = 0;
    const uint8_t pol[][2] = {
        { 1, 1 }, // power HIGH, reset HIGH
        { 1, 0 }, // power HIGH, reset LOW
        { 0, 1 }, // power LOW, reset HIGH
        { 0, 0 }, // power LOW, reset LOW
    };

    for (int i = 0; i < 4 && !detected; i++) {
        printf("[4.%d] power=%d reset=%d: ", pol[i][0], pol[i][1]);

        // Power cycle
        omv_gpio_write(OMV_CSI_POWER_PIN, pol[i][0]);
        mp_hal_delay_ms(10);
        omv_gpio_write(OMV_CSI_POWER_PIN, !pol[i][0]);
        mp_hal_delay_ms(100);

        // Reset pulse
        omv_gpio_write(OMV_CSI_RESET_PIN, pol[i][1]);
        mp_hal_delay_ms(10);
        omv_gpio_write(OMV_CSI_RESET_PIN, !pol[i][1]);
        mp_hal_delay_ms(100);

        // Read product ID
        pid_h = 0; pid_l = 0;
        test_i2c_read_reg(0x300A, &pid_h);
        test_i2c_read_reg(0x300B, &pid_l);
        printf("PID=0x%02X 0x%02X\n", pid_h, pid_l);

        if (pid_h == 0x56) {
            detected = 1;
            printf("[4.%d] SUCCESS! OV5640 detected (power=%d reset=%d)\n",
                   i, pol[i][0], pol[i][1]);
        }
    }

    if (!detected) {
        printf("[4] FAILED: OV5640 not reachable on any polarity\n");
        return;
    }

    // ===== Step 5: Write MIPI sensor registers =====
    int rc = test_i2c_write_array(mipi_init_regs);
    printf("[5] Write MIPI regs: %d\n", rc);
    R_BSP_SoftwareDelay(50, BSP_DELAY_UNITS_MILLISECONDS);

    // ===== Step 6: Configure PLL =====
    test_ov5640_configure_clocks();
    printf("[6] PLL configured\n");

    // ===== Step 7: Set MIPI virtual channel =====
    test_ov5640_set_mipi_vc(0);
    printf("[7] Virtual channel set\n");

    R_BSP_SoftwareDelay(50, BSP_DELAY_UNITS_MILLISECONDS);

    // ===== Step 8: Stream off before VIN open (titan sequence) =====
    test_ov5640_stream_off();
    R_BSP_SoftwareDelay(5, BSP_DELAY_UNITS_MILLISECONDS);
    printf("[8] Stream off\n");

    // ===== Step 9: VIN Open (QVGA runtime config) =====
    // g_cam_vin_cfg is const (FSP-generated common_data.c). To make the VIN
    // preclip / UDS clipping match the 320x240 sensor output WITHOUT editing
    // common_data.c (FSP regen would silently revert it, and it would also
    // change the OMV boot baseline), use a runtime copy with QVGA dims.
    // Same pattern as the I2C config copy above.
    static vin_extended_cfg_t vin_cfg_ext = {0};
    static capture_cfg_t vin_cfg = {0};
    vin_cfg_ext = *(const vin_extended_cfg_t *) g_cam_vin_cfg.p_extend; // copy const extended cfg
    vin_cfg_ext.input_ctrl.preclip.pixel_end = 319;   // 320 - 1
    vin_cfg_ext.input_ctrl.preclip.line_end = 239;    // 240 - 1
    vin_cfg_ext.conversion_data.uds_clipping_bits.cl_hsize = 320;
    vin_cfg_ext.conversion_data.uds_clipping_bits.cl_vsize = 240;
    vin_cfg = g_cam_vin_cfg;                    // copy const capture cfg
    vin_cfg.p_extend = &vin_cfg_ext;            // point at QVGA extended cfg
    err = R_VIN_Open(&g_cam_vin_ctrl, &vin_cfg);
    printf("[9] R_VIN_Open (preclip 320x240): %d\n", err);
    if (err != FSP_SUCCESS) {
        printf("[9] FAILED: VIN open error %d\n", err);
        return;
    }

    // ===== Step 10: Stream on =====
    R_BSP_SoftwareDelay(5, BSP_DELAY_UNITS_MILLISECONDS);
    test_ov5640_stream_on();
    printf("[10] Stream on\n");

    // ===== Step 11: Start capture =====
    err = R_VIN_CaptureStart(&g_cam_vin_ctrl, NULL);
    printf("[11] R_VIN_CaptureStart: %d\n", err);

    // ===== Step 12: Wait for callback =====
    printf("[12] Waiting for VIN callback (5 seconds)...\n");
    g_test_vin_frame_count = 0;
    for (int i = 0; i < 50; i++) {
        R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MILLISECONDS);
        if (g_test_vin_frame_count > 0) {
            printf("SUCCESS! VIN callback fired %d times, buffer=%p\n",
                   g_test_vin_frame_count, g_test_vin_last_buffer);
            break;
        }
    }

    if (g_test_vin_frame_count == 0) {
        printf("FAILED: VIN callback never fired\n");
    }

    printf("===== Test Done =====\n");
}

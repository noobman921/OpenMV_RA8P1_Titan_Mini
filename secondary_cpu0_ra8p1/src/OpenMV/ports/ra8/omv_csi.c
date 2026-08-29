#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "py/mphal.h"
//#include "irq.h"
#include "omv_boardconfig.h"
#include "unaligned_memcpy.h"
#include "omv_gpio.h"
#include "omv_i2c.h"
#include "omv_csi.h"

// VIN callback - 完全模仿 Titan cam_vin_callback:
// frame_complete 时校验并记录完成 buffer 指针 + 递增帧序列。
// VIN 以 use_runtime_buffer=0 自动连续采集, 无需在此重启捕获。
// VIN callback diagnostics (ISR-safe: volatile globals, printed in main loop)
static volatile uint32_t g_vin_frame_complete = 0;
static volatile uint32_t g_vin_buf_valid = 0;
static volatile uint32_t g_vin_callback_count = 0;
static volatile uint32_t g_vin_last_tick = 0;
static volatile uint32_t g_vin_interval_ms = 0;


// dmac
transfer_info_t omv_csi_transfer_info_normal =
 {
     .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,  //每次传输后，目标地址指针都会增加
     .transfer_settings_word_b.repeat_area    = TRANSFER_REPEAT_AREA_SOURCE,     //源区域重复 (正常模式下无效)
     .transfer_settings_word_b.irq            = TRANSFER_IRQ_END,                //传输完成后中断
     .transfer_settings_word_b.chain_mode     = TRANSFER_CHAIN_MODE_DISABLED,    //不使能（DMAC没有该功能，仅DTC有）
     .transfer_settings_word_b.src_addr_mode  = TRANSFER_ADDR_MODE_INCREMENTED,  //每次传输后，源地址指针都会增加
     .transfer_settings_word_b.size           = TRANSFER_SIZE_8_BYTE,            //每次传输4字节
     .transfer_settings_word_b.mode           = TRANSFER_MODE_NORMAL,            //正常传输模式
     .p_dest                                  = (void *) NULL,             //目标地址
     .p_src                                   = (void const *) NULL,       //源地址
     .num_blocks = 0,            //指定传输的块数（正常模式下无效，仅在重复、块或重复-块模式下有效）
     .length     = 0,  //指定传输的长度（即正常和重复模式下的传输次数 或 块和重复-块模式下传输的块大小）
 };
volatile bool omv_csi_dmac_complete = false;

// VIN runtime mutable config (copied from FSP-generated const config at init)
capture_cfg_t OMV_VIN_CFG;
vin_extended_cfg_t OMV_VIN_CFG_EXTEND;

// 数组接收: VIN 以 use_runtime_buffer=0 连续采集到 FSP 静态
// vin_image_buffer_1/2/3 (SDRAM no-cache)。callback 只记录最新完成的 buffer
// 指针和帧序列, snapshot 轮询帧序列变化后拷贝最新帧到 framebuffer。
static volatile uint8_t *p_camera_capture_buffer_stored = NULL;
static volatile uint32_t g_camera_frame_sequence = 0;
static volatile uint32_t g_camera_frame_sequence_processed = 0;

// 静态模式 VIN 需一次 CaptureStart 启动连续采集; 每次 R_VIN_Open 后需重新启动。
static bool s_vin_capture_started = false;

// 校验 p_buffer 是否落在 FSP 静态 buffer 数组范围内
static bool camera_capture_buffer_is_valid(uint8_t const * p_buffer)
{
    if (NULL == p_buffer) return false;
    if (p_buffer == (uint8_t const *) &vin_image_buffer_1[0]) return true;
    if (p_buffer == (uint8_t const *) &vin_image_buffer_2[0]) return true;
    if (p_buffer == (uint8_t const *) &vin_image_buffer_3[0]) return true;
    return false;
}


//中断一次采集
static int ra_csi_abort(omv_csi_t *csi, bool fifo_flush, bool in_irq){
    fsp_err_t err;
    err = R_VIN_Close(&g_cam_vin_ctrl);
    if (err != FSP_SUCCESS) {
        return -1;
    }
    err = R_VIN_Open(&g_cam_vin_ctrl, &OMV_VIN_CFG);
    if (err != FSP_SUCCESS) {
        return -1;
    }
    // 静态模式需重新启动采集
    s_vin_capture_started = false;
    return 0;
}

//csi配置
static int ra_csi_config(omv_csi_t *csi, omv_csi_config_t config){
    fsp_err_t err;
    if(config == OMV_CSI_CONFIG_INIT){
        err = R_VIN_Open(&g_cam_vin_ctrl, &OMV_VIN_CFG);
        if (err != FSP_SUCCESS) {
            return -1;
        }
        s_vin_capture_started = false;
    }
    else if(config == OMV_CSI_CONFIG_DEINIT){
        err = R_VIN_Close(&g_cam_vin_ctrl);
        if (err != FSP_SUCCESS) {
            return -1;
        }
        s_vin_capture_started = false;
    }
    else if(config == OMV_CSI_CONFIG_FRAMESIZE || config == OMV_CSI_CONFIG_PIXFORMAT){
        // 直接使用 FSP 生成的 g_cam_vin_cfg 的运行时拷贝
        // VIN 按 FSP 配置工作: VGA 640x480 输入 -> 无缩放, CSC YCbCr->RGB565
        // 输出 640x480 到静态 vin_image_buffer_1/2/3。
        err = R_VIN_Close(&g_cam_vin_ctrl);
        if (err != FSP_SUCCESS) {
            return -1;
        }

        err = R_VIN_Open(&g_cam_vin_ctrl, &OMV_VIN_CFG);
        if (err != FSP_SUCCESS) {
            return -1;
        }
        s_vin_capture_started = false;
    }

    return 0;
}

static int ra_csi_shutdown(omv_csi_t *csi, int enable) {
    int ret = 0;
    if (enable) {
        ret = omv_csi_config(csi, OMV_CSI_CONFIG_DEINIT);
    } else {
        ret = omv_csi_config(csi, OMV_CSI_CONFIG_INIT);
    }
    return ret;
}

//获取一帧数据 - 轮询帧序列变化, 拷贝最新静态数组到 framebuffer
static int ra_csi_snapshot(omv_csi_t *csi, image_t *image, uint32_t flags){
    framebuffer_t *fb = csi->fb;
    size_t frame_size = fb->u * fb->v * 2;  // VGA 640x480 RGB565 = 614400

    mp_int_t point0 = mp_hal_ticks_ms();
    // 静态模式: 首次 (或每次 Open 后) 需启动一次连续采集, VIN 自动轮转 buffer
    if (!s_vin_capture_started) {
        omv_csi_t *csi_local = omv_csi_get(-1);
        omv_i2c_write_reg(csi_local->i2c, csi_local->slv_addr, 0x4202, 2, 0x00, 1);
        mp_hal_delay_ms(5);

        if (R_VIN_CaptureStart(&g_cam_vin_ctrl, NULL) != FSP_SUCCESS) {
            return OMV_CSI_ERROR_CAPTURE_FAILED;
        }
        s_vin_capture_started = true;
    }

    mp_int_t point1 = mp_hal_ticks_ms();
    // 等待新帧
     uint32_t wait_loops = 0;
     for (mp_uint_t start = mp_hal_ticks_ms(); ; mp_event_handle_nowait(), wait_loops++) {
         if (g_camera_frame_sequence != g_camera_frame_sequence_processed) {
             break;
         }
         if (flags & OMV_CSI_FLAG_NON_BLOCK) {
             return OMV_CSI_ERROR_WOULD_BLOCK;
         }
         if ((mp_hal_ticks_ms() - start) > OMV_CSI_TIMEOUT_MS) {
             return OMV_CSI_ERROR_CAPTURE_TIMEOUT;
         }
     }

    g_camera_frame_sequence_processed = g_camera_frame_sequence;

    mp_int_t point2 = mp_hal_ticks_ms();
    // 从最新静态数组拷贝到 framebuffer 空闲 buffer
    vbuffer_t *buffer = framebuffer_acquire(fb, FB_FLAG_FREE | FB_FLAG_PEEK);
    if (buffer == NULL) {
        return OMV_CSI_ERROR_FRAMEBUFFER_ERROR;
    }

     // dmac: NORMAL 模式分段搬运, 自适应 frame_size。
    // 每段次数 = min(剩余, 65535), 循环直到搬完 frame_size 字节。
    {
        const uint32_t bytes_per_xfer = 8;   // TRANSFER_SIZE_8_BYTE
        const uint32_t max_len        = 0xFFFF;  // uint16_t length 上限
        const uint32_t total_transfers = frame_size / bytes_per_xfer;
        uint8_t *p_dst = (uint8_t *) buffer->data;
        const uint8_t *p_src = (const uint8_t *) p_camera_capture_buffer_stored;

        for (uint32_t remaining = total_transfers; remaining > 0; ) {
            uint32_t chunk = (remaining > max_len) ? max_len : remaining;
            omv_csi_transfer_info_normal.p_dest = p_dst;
            omv_csi_transfer_info_normal.p_src  = p_src;
            omv_csi_transfer_info_normal.length = chunk;
            omv_csi_dmac_complete = false;

            if (R_DMAC_Disable(&omv_csi_dmac_transfer_ctrl) != FSP_SUCCESS) {
                return OMV_CSI_ERROR_CAPTURE_FAILED;
            }
            if (R_DMAC_Reconfigure(&omv_csi_dmac_transfer_ctrl, &omv_csi_transfer_info_normal) != FSP_SUCCESS) {
                return OMV_CSI_ERROR_CAPTURE_FAILED;
            }
            if (R_DMAC_Enable(&omv_csi_dmac_transfer_ctrl) != FSP_SUCCESS) {
                return OMV_CSI_ERROR_CAPTURE_FAILED;
            }
            if (R_DMAC_SoftwareStart(&omv_csi_dmac_transfer_ctrl, TRANSFER_START_MODE_REPEAT) != FSP_SUCCESS) {
                return OMV_CSI_ERROR_CAPTURE_FAILED;
            }
            while (false == omv_csi_dmac_complete) {
                mp_event_handle_nowait();
            }

            p_dst += chunk * bytes_per_xfer;
            p_src += chunk * bytes_per_xfer;
            remaining -= chunk;
        }
    }
    SCB_InvalidateDCache_by_Addr((uint32_t *)buffer->data, frame_size);
    mp_int_t point3 = mp_hal_ticks_ms();

//    memcpy(buffer->data, (const void *) p_camera_capture_buffer_stored, frame_size);
    framebuffer_release(fb, FB_FLAG_FREE);

    // 更新 fb 元数据并构建 image
    csi->fb->w = csi->transpose ? csi->fb->v : csi->fb->u;
    csi->fb->h = csi->transpose ? csi->fb->u : csi->fb->v;
    csi->fb->pixfmt = csi->pixformat;

    framebuffer_to_image(csi->fb, image);
    mp_int_t point4 = mp_hal_ticks_ms();
    printf("ra_csi_snapshot: VIN start=%dms, wait=%dms, dmac=%dms, total=%dms seq=%lu\n",
           point1 - point0, point2 - point1, point3 - point2, point4 - point0,
            g_camera_frame_sequence);
    return 0;
}

int ra_csi_isp_reset(omv_csi_t *csi) {
    return 0;
}

//获取GPT定时器频率
static uint32_t ra_clk_get_frequency(omv_clk_t *clk){
    fsp_err_t err;
    timer_info_t p_info;
    err = R_GPT_InfoGet(&OMV_GPT_TIM_HANDLE, &p_info);
    if (err != FSP_SUCCESS) {
        return 0;
    }
    return p_info.clock_frequency / p_info.period_counts;
}

//设置GPT定时器频率
static int ra_clk_set_frequency(omv_clk_t *clk, uint32_t frequency){
    fsp_err_t err;
    timer_info_t p_info;
    err = R_GPT_InfoGet(&OMV_GPT_TIM_HANDLE, &p_info);
    if (err != FSP_SUCCESS) {
        return -1;
    }
    uint32_t clock_frequency = p_info.clock_frequency;
    uint32_t period_counts = clock_frequency / frequency;

    err = R_GPT_Stop(&OMV_GPT_TIM_HANDLE);
    if (err != FSP_SUCCESS) return -1;

    err = R_GPT_PeriodSet(&OMV_GPT_TIM_HANDLE, period_counts);
    if (err != FSP_SUCCESS) return -1;

    err = R_GPT_DutyCycleSet(&OMV_GPT_TIM_HANDLE, period_counts / 2, OMV_GPT_PIN);
    if (err != FSP_SUCCESS) return -1;

    err = R_GPT_Start(&OMV_GPT_TIM_HANDLE);
    if (err != FSP_SUCCESS) return -1;
    return 0;
}




void cam_vin_callback(capture_callback_args_t *p_args){
    vin_event_t            event            = (vin_event_t) p_args->event;
    vin_interrupt_status_t interrupt_status = (vin_interrupt_status_t) p_args->interrupt_status;

    // Diagnostics: measure VIN interrupt interval
    g_vin_callback_count++;
    uint32_t now = xTaskGetTickCountFromISR();
    if (g_vin_last_tick != 0) {
        g_vin_interval_ms = (now - g_vin_last_tick) * portTICK_PERIOD_MS;
    }
    g_vin_last_tick = now;

    if(event == VIN_EVENT_NOTIFY)
    {
        if (interrupt_status.bits.frame_complete)
        {
            g_vin_frame_complete++;

            /* Capture Complete - Process data buffer pointer and index */
            if (camera_capture_buffer_is_valid(p_args->p_buffer))
            {
                g_vin_buf_valid = 1;
                p_camera_capture_buffer_stored = p_args->p_buffer;
                g_camera_frame_sequence++;
            }
            else
            {
                g_vin_buf_valid = 0;
            }
        }
    }
}


void cam_mipi_csi_callback(mipi_csi_callback_args_t *p_args) {
}

// dmac callback
void omv_csi_dmac_callback(transfer_callback_args_t *p_args){
    omv_csi_dmac_complete = true;
}

int omv_csi_ops_init(omv_csi_t *csi) {
    fsp_err_t err = FSP_SUCCESS;

    // use_runtime_buffer=0: VIN 自动连续采集到静态 vin_image_buffer_1/2/3 (SDRAM),
    // callback 记录完成 buffer, snapshot 轮询后拷贝到 framebuffer。
    OMV_VIN_CFG = OMV_VIN_CFG_;
    OMV_VIN_CFG_EXTEND = OMV_VIN_CFG_EXTEND_;

    // FSP 默认 use_runtime_buffer=0 (静态数组接收);
    OMV_VIN_CFG_EXTEND.output_ctrl.use_runtime_buffer = 0;
    OMV_VIN_CFG.p_extend = &OMV_VIN_CFG_EXTEND;

    // Init modules
    err = R_VIN_Open(&g_cam_vin_ctrl, &OMV_VIN_CFG);
    if(err != FSP_SUCCESS){
        return -1;
    }
    err = R_GPT_Open(&OMV_GPT_TIM_HANDLE, &OMV_GPT_TIM_CFG);
    if(err != FSP_SUCCESS){
        return -1;
    }
    err = R_GPT_Start(&OMV_GPT_TIM_HANDLE);
    if(err != FSP_SUCCESS){
        return -1;
    }
    err = R_DMAC_Open(&omv_csi_dmac_transfer_ctrl, &omv_csi_dmac_transfer_cfg);
    if(err != FSP_SUCCESS){
        return -1;
    }

    // Reset frame tracking
    p_camera_capture_buffer_stored = NULL;
    g_camera_frame_sequence = 0;
    g_camera_frame_sequence_processed = 0;

    // Set CSI ops.
    csi->abort = ra_csi_abort;
    csi->config = ra_csi_config;
    csi->shutdown = ra_csi_shutdown;
    csi->snapshot = ra_csi_snapshot;
    csi->isp_reset = ra_csi_isp_reset;

    // Set CSI clock ops.
    csi->clk->freq = OMV_CSI_CLK_FREQUENCY;
    csi->clk->set_freq = ra_clk_set_frequency;
    csi->clk->get_freq = ra_clk_get_frequency;

    return 0;
}

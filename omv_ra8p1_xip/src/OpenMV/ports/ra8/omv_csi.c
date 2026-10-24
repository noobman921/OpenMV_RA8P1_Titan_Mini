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
#include "ra8_capture.h"

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

// 把 VIN 的接收几何对齐到**传感器输出尺寸**（framesize 分辨率）。
// 必须在所有 R_VIN_Open() 之前调用：R_VIN_Open 会把这些字段直接写进寄存器
// (ra/fsp/src/r_vin/r_vin.c:92-96,125-127)，中间没有缓存，改 struct 即生效。
// ra_gen/common_data.c 与 configuration.xml 都不动 —— FSP 重新生成会覆盖它们，
// 而运行期覆盖已经够用（同一模式见 ports/ra8/mipi_csi_test.c 的 QVGA 试验）。
// 六个字段每次都全写，不做"值变了才写"的判断：这样 OMV_VIN_CFG_EXTEND 永远
// 不会相对 FSP 模板漂移，来回切 VGA/QVGA 也是幂等的。
//
// 注意这里读的是 csi->resolution[framesize] 而不是 csi->fb->u/v：fb->u/v 会被
// sensor.set_windowing() 改写成 ROI 尺寸，而本端口**没有**处理
// OMV_CSI_CONFIG_WINDOWING（见 ra_csi_config），VIN 从不参与 ROI 裁剪。
// 用 framesize 分辨率可保证 windowing 的行为与改动前逐字节一致。
static void omv_vin_apply_geometry(omv_csi_t *csi)
{
    uint16_t w = 640;
    uint16_t h = 480;

    // framesize 尚未设定时保持 FSP 生成的 VGA 基线。
    if (csi != NULL && csi->framesize >= OMV_CSI_FRAMESIZE_CUSTOM &&
        csi->framesize < OMV_CSI_FRAMESIZE_MAX) {
        uint16_t rw = csi->resolution[csi->framesize][0];
        uint16_t rh = csi->resolution[csi->framesize][1];
        if (rw > 0 && rh > 0) {
            w = rw;
            h = rh;
        }
    }

    OMV_VIN_CFG_EXTEND.input_ctrl.preclip.pixel_start = 0;
    OMV_VIN_CFG_EXTEND.input_ctrl.preclip.pixel_end   = w - 1;
    OMV_VIN_CFG_EXTEND.input_ctrl.preclip.line_start  = 0;
    OMV_VIN_CFG_EXTEND.input_ctrl.preclip.line_end    = h - 1;

    // 行距必须等于行像素数：ra_csi_snapshot 的 DMAC 是从 buffer 起始**连续**拷
    // fb->u*fb->v*2 字节，stride 留成 640 会拷出隔行交错的半幅数据。
    // 320/640 都是 16 的倍数，满足 16bpp 下 32 字节的写边界。
    OMV_VIN_CFG_EXTEND.input_ctrl.image_stride = w;

    OMV_VIN_CFG_EXTEND.conversion_data.uds_clipping_bits.cl_hsize = w;
    OMV_VIN_CFG_EXTEND.conversion_data.uds_clipping_bits.cl_vsize = h;
}

// 数组接收: VIN 以 use_runtime_buffer=0 连续采集到 FSP 静态
// vin_image_buffer_1/2/3 (SDRAM no-cache)。callback 只记录最新完成的 buffer
// 指针和帧序列, snapshot 轮询帧序列变化后拷贝最新帧到 framebuffer。
static volatile uint8_t *p_camera_capture_buffer_stored = NULL;
static volatile uint32_t g_camera_frame_sequence = 0;
static volatile uint32_t g_camera_frame_sequence_processed = 0;

// 静态模式 VIN 需一次 CaptureStart 启动连续采集; 每次 R_VIN_Open 后需重新启动。
static bool s_vin_capture_started = false;

// 采集流水线开关 (对应 Titan ra8_capture_pipeline)。
// 关闭时 snapshot 返回前 abort, VIN 不在后台连续采集 —— 用于隔离图像算子基准测试。
static bool pipeline_enabled = true;

bool ra8_capture_pipeline(int enable) {
    bool old = pipeline_enabled;
    if (enable >= 0) {
        pipeline_enabled = enable != 0;
    }
    return old;
}

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
    // 先让传感器停流（Titan titan_camera_close() 同做法）。否则 VIN/MIPI 关闭后
    // 数据仍持续灌进已关闭的接收端，重开后的头几帧是坏帧，表现为偶发花屏/撕裂。
    if (csi != NULL) {
        omv_i2c_write_reg(csi->i2c, csi->slv_addr, 0x4202, 2, 0x0F, 1);
    }
    err = R_VIN_Close(&g_cam_vin_ctrl);
    if (err != FSP_SUCCESS) {
        return -1;
    }
    omv_vin_apply_geometry(csi);
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
        omv_vin_apply_geometry(csi);
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
        // VIN 按 FSP 配置工作: 传感器输出 (VGA 或 QVGA) -> 无缩放, CSC YCbCr->RGB565
        // -> 静态 vin_image_buffer_1/2/3 (各 614400 B, 足够容纳任一档)。
        // 几何跟随当前 framesize 的传感器输出尺寸, 见 omv_vin_apply_geometry。
        err = R_VIN_Close(&g_cam_vin_ctrl);
        if (err != FSP_SUCCESS) {
            return -1;
        }

        omv_vin_apply_geometry(csi);
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
    // 随 framesize 变化 (VGA 640x480x2 = 614400, QVGA 320x240x2 = 153600);
    // 由 omv_csi_set_framesize -> omv_csi_set_framebuffers 保证 buf_size >= 此值。
    // 两档都是 32 的倍数, 与下面按 cache line 做的失效范围一致。
    size_t frame_size = fb->u * fb->v * 2;

    // 静态模式: 首次 (或每次 Open 后) 需启动一次连续采集, VIN 自动轮转 buffer
    // 顺序对齐 Titan titan_camera_start(): CaptureStart → 5ms → 放行出流。
    // 不再重写 0x3008（唤醒位）—— 复位阶段已完成，重复写会让传感器重新上电，
    // 把 AEC/AWB 的收敛状态清掉。
    if (!s_vin_capture_started) {
        if (R_VIN_CaptureStart(&g_cam_vin_ctrl, NULL) != FSP_SUCCESS) {
            return OMV_CSI_ERROR_CAPTURE_FAILED;
        }
        mp_hal_delay_ms(5);
        omv_i2c_write_reg(csi->i2c, csi->slv_addr, 0x4202, 2, 0x00, 1);
        s_vin_capture_started = true;
    }

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

//    memcpy(buffer->data, (const void *) p_camera_capture_buffer_stored, frame_size);
    framebuffer_release(fb, FB_FLAG_FREE);

    // 更新 fb 元数据并构建 image
    csi->fb->w = csi->transpose ? csi->fb->v : csi->fb->u;
    csi->fb->h = csi->transpose ? csi->fb->u : csi->fb->v;
    csi->fb->pixfmt = csi->pixformat;

    framebuffer_to_image(csi->fb, image);

    // 预览推送。
    // 1.7.3 的核心只在 flags 带 OMV_CSI_FLAG_UPDATE_FB 时才推预览，而这个位
    // 只有旧 sensor 模块（py_csi.c）会设；csi 新 API（py_csi_ng.c）传 flags=0，
    // 于是走 csi.snapshot() 的脚本永远不更新 IDE 预览 —— 上游后续版本把判定
    // 反过来（默认推、NO_UPDATE 关）才修掉，但本工程核还是 1.7.3 的。
    // 在这里补：旧 API 由核心推（此分支跳过），新 API 由端口推，同一帧只编码一次。
    if (!(flags & OMV_CSI_FLAG_UPDATE_FB)) {
        framebuffer_update_preview(image);
    }

    // 流水线关闭时本帧用完即停: VIN 回到未启动状态, 不占后台 DMA。
    // 下次 snapshot 会因 s_vin_capture_started=false 重新 CaptureStart。
    if (!pipeline_enabled && csi->abort(csi, false, false)) {
        return OMV_CSI_ERROR_CAPTURE_FAILED;
    }
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

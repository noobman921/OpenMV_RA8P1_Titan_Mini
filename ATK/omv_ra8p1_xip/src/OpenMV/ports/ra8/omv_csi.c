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

// VIN runtime mutable config (copied from FSP-generated const config at init)
capture_cfg_t OMV_CEU_CFG;

static bool s_vin_capture_started = false;

// 连续采集（乒乓）：每帧结束后由 CEU 中断直接把下一帧挂上，不再等任务端重新触发。
static framebuffer_t *s_ceu_fb = NULL;       // 当前使用的 framebuffer
static vbuffer_t     *s_ceu_buf = NULL;      // CEU 正在写的 buffer（不在任何队列里）
static volatile bool  s_continuous = false;  // 接力开关

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
    OMV_CEU_CFG.x_capture_start_pixel = 0;
    OMV_CEU_CFG.x_capture_pixels = w;
    OMV_CEU_CFG.y_capture_start_pixel = 0;
    OMV_CEU_CFG.y_capture_pixels = h;

}


// 把 CEU 正在写的那块归还到 free 队列。必须在 R_CEU_Close 之后调用。
static void ra_ceu_buf_release(omv_csi_t *csi)
{
    framebuffer_t *fb = (s_ceu_fb != NULL) ? s_ceu_fb : ((csi != NULL) ? csi->fb : NULL);

    if (s_ceu_buf != NULL && fb != NULL) {
        framebuffer_reset(s_ceu_buf);
        queue_push(fb->free_queue, s_ceu_buf);
    }
    s_ceu_buf = NULL;
    s_ceu_fb = NULL;
}

//中断一次采集
static int ra_csi_abort(omv_csi_t *csi, bool fifo_flush, bool in_irq){
    fsp_err_t err;
    s_continuous = false;   // 先停接力，避免 Close 期间中断里再挂帧
    // 先让传感器停流（Titan titan_camera_close() 同做法）。否则 VIN/MIPI 关闭后
    // 数据仍持续灌进已关闭的接收端，重开后的头几帧是坏帧，表现为偶发花屏/撕裂。
    if (csi != NULL) {
        omv_i2c_write_reg(csi->i2c, csi->slv_addr, 0x4202, 2, 0x0F, 1);
    }
//    err = R_VIN_Close(&g_cam_vin_ctrl);
    err = R_CEU_Close(&g_ceu_omv_ctrl);

    if (err != FSP_SUCCESS) {
        return -1;
    }
    ra_ceu_buf_release(csi);
    omv_vin_apply_geometry(csi);
//    err = R_VIN_Open(&g_cam_vin_ctrl, &OMV_VIN_CFG);
    err = R_CEU_Open(&g_ceu_omv_ctrl, &OMV_CEU_CFG);
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
    s_continuous = false;
    if(config == OMV_CSI_CONFIG_INIT){
        ra_ceu_buf_release(csi);
        omv_vin_apply_geometry(csi);
//        err = R_VIN_Open(&g_cam_omv_ctrl, &OMV_VIN_CFG);
        err = R_CEU_Open(&g_ceu_omv_ctrl, &OMV_CEU_CFG);
        if (err != FSP_SUCCESS) {
            return -1;
        }
        s_vin_capture_started = false;
    }
    else if(config == OMV_CSI_CONFIG_DEINIT){
//        err = R_VIN_Close(&g_cam_vin_ctrl);
        err = R_CEU_Close(&g_ceu_omv_ctrl);
        if (err != FSP_SUCCESS) {
            return -1;
        }
        ra_ceu_buf_release(csi);
        s_vin_capture_started = false;
    }
    else if(config == OMV_CSI_CONFIG_FRAMESIZE || config == OMV_CSI_CONFIG_PIXFORMAT){
        // 直接使用 FSP 生成的 g_cam_vin_cfg 的运行时拷贝
        // VIN 按 FSP 配置工作: 传感器输出 (VGA 或 QVGA) -> 无缩放, CSC YCbCr->RGB565
        // -> 静态 vin_image_buffer_1/2/3 (各 614400 B, 足够容纳任一档)。
        // 几何跟随当前 framesize 的传感器输出尺寸, 见 omv_vin_apply_geometry。
//        err = R_VIN_Close(&g_cam_vin_ctrl);
        err = R_CEU_Close(&g_ceu_omv_ctrl);
        if (err != FSP_SUCCESS) {
            return -1;
        }
        ra_ceu_buf_release(csi);

        omv_vin_apply_geometry(csi);
//        err = R_VIN_Open(&g_cam_vin_ctrl, &OMV_VIN_CFG);
        err = R_CEU_Open(&g_ceu_omv_ctrl, &OMV_CEU_CFG);
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

//获取一帧数据 - 连续采集：ISR 在帧结束后直接把下一帧挂给 CEU
static int ra_csi_snapshot(omv_csi_t *csi, image_t *image, uint32_t flags){
    framebuffer_t *fb = csi->fb;
    size_t frame_size = fb->u * fb->v * 2;

    // 传感器出帧
    if (!s_vin_capture_started) {
        omv_i2c_write_reg(csi->i2c, csi->slv_addr, 0x4202, 2, 0x00, 1);
        s_vin_capture_started = true;
    }

    // 第一次（或 abort/config 之后）：取一块空闲 buffer 启动接力
    if (!s_continuous) {
        vbuffer_t *first = framebuffer_acquire(fb, FB_FLAG_FREE);
        if (first == NULL) {
            return OMV_CSI_ERROR_FRAMEBUFFER_ERROR;
        }
        s_ceu_fb = fb;
        s_ceu_buf = first;
        if (R_CEU_CaptureStart(&g_ceu_omv_ctrl, (uint8_t *) first->data) != FSP_SUCCESS) {
            s_ceu_buf = NULL;
            return OMV_CSI_ERROR_CAPTURE_FAILED;
        }
        s_continuous = true;
    }

    // 等一帧完成：完成的 buffer 由 CEU 中断推进 used 队列
    for (mp_uint_t start = mp_hal_ticks_ms(); queue_is_empty(fb->used_queue); mp_event_handle_nowait()) {
        if (flags & OMV_CSI_FLAG_NON_BLOCK) {
            return OMV_CSI_ERROR_WOULD_BLOCK;
        }
        if ((mp_hal_ticks_ms() - start) > OMV_CSI_TIMEOUT_MS) {
            s_continuous = false;
            (void) ra_csi_abort(csi, true, false);
            return OMV_CSI_ERROR_CAPTURE_TIMEOUT;
        }
    }

    // 只保留最新一帧，避免交付积压的旧帧
    while (queue_size(fb->used_queue) > 1) {
        vbuffer_t *old = queue_pop(fb->used_queue, false);
        if (old == NULL) {
            break;
        }
        framebuffer_reset(old);
        queue_push(fb->free_queue, old);
    }

    // 更新 fb 元数据并构建 image
    csi->fb->w = csi->transpose ? csi->fb->v : csi->fb->u;
    csi->fb->h = csi->transpose ? csi->fb->u : csi->fb->v;
    csi->fb->pixfmt = csi->pixformat;

    framebuffer_to_image(csi->fb, image);
    if (image != NULL && image->data != NULL) {
        SCB_InvalidateDCache_by_Addr((uint32_t *) image->data, frame_size);
    }

    // 预览推送。
    // 1.7.3 的核心只在 flags 带 OMV_CSI_FLAG_UPDATE_FB 时才推预览，而这个位
    // 只有旧 sensor 模块（py_csi.c）会设；csi 新 API（py_csi_ng.c）传 flags=0，
    // 于是走 csi.snapshot() 的脚本永远不更新 IDE 预览 —— 上游后续版本把判定
    // 反过来（默认推、NO_UPDATE 关）才修掉，但本工程核还是 1.7.3 的。
    // 在这里补：旧 API 由核心推（此分支跳过），新 API 由端口推，同一帧只编码一次。
    if (!(flags & OMV_CSI_FLAG_UPDATE_FB)) {
        framebuffer_update_preview(image);
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

/* Callback function */
void g_ceu_omv_callback(capture_callback_args_t *p_args)
{
    if (!(p_args->event & CEU_EVENT_FRAME_END)) {
        return;
    }

    if (!s_continuous || s_ceu_buf == NULL) {
        return;
    }

    // 乒乓：完成的这块推给 used 队列，下一块挂给 CEU，全程不经过任务端
    framebuffer_t *fb = s_ceu_fb;
    vbuffer_t *done = s_ceu_buf;
    vbuffer_t *next = framebuffer_acquire(fb, FB_FLAG_FREE);

    if (next == NULL) {
        // 没有空闲 buffer：原地覆盖，放弃这一帧的交付（任务端还没消费完）
        next = done;
    } else {
        if (!queue_push(fb->used_queue, done)) {
            // used 队列满：丢掉最旧的一帧
            vbuffer_t *old = queue_pop(fb->used_queue, false);
            if (old != NULL) {
                framebuffer_reset(old);
                queue_push(fb->free_queue, old);
            }
            queue_push(fb->used_queue, done);
        }
    }

    s_ceu_buf = next;
    if (R_CEU_CaptureStart(&g_ceu_omv_ctrl, (uint8_t *) next->data) != FSP_SUCCESS) {
        // 挂下一帧失败（例如 CE 还没清）——停止接力，任务端下次 snapshot 会重启
        framebuffer_reset(next);
        queue_push(fb->free_queue, next);
        s_ceu_buf = NULL;
        s_continuous = false;
    }
}

// dmac callback
void omv_csi_dmac_callback(transfer_callback_args_t *p_args){

}

int omv_csi_ops_init(omv_csi_t *csi) {
    fsp_err_t err = FSP_SUCCESS;

    // use_runtime_buffer=0: VIN 自动连续采集到静态 vin_image_buffer_1/2/3 (SDRAM),
    // callback 记录完成 buffer, snapshot 轮询后拷贝到 framebuffer。
//    OMV_VIN_CFG = OMV_VIN_CFG_;
    OMV_CEU_CFG = g_ceu_omv_cfg;

    // Init modules
    err = R_CEU_Open(&g_ceu_omv_ctrl, &OMV_CEU_CFG);
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
    s_continuous = false;
    s_ceu_buf = NULL;
    s_ceu_fb = NULL;

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

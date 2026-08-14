/* generated thread header file - do not edit */
#ifndef DOWNLOADER_THREAD_H_
#define DOWNLOADER_THREAD_H_
#include "bsp_api.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "hal_data.h"
#ifdef __cplusplus
                extern "C" void downloader_thread_entry(void * pvParameters);
                #else
extern void downloader_thread_entry(void *pvParameters);
#endif
#include "r_ospi_b.h"
#include "r_spi_flash_api.h"
#include "r_dmac.h"
#include "r_transfer_api.h"
#include "r_usb_basic.h"
#include "r_usb_basic_api.h"
#include "r_usb_pcdc_api.h"
FSP_HEADER
#if OSPI_B_CFG_DMAC_SUPPORT_ENABLE
#include "r_dmac.h"
#endif
#if OSPI_CFG_DOTF_SUPPORT_ENABLE
#include "r_sce_if.h"
#endif

extern const spi_flash_instance_t g_ospi0;
extern ospi_b_instance_ctrl_t g_ospi0_ctrl;
extern const spi_flash_cfg_t g_ospi0_cfg;
/* Transfer on DMAC Instance. */
extern const transfer_instance_t g_transfer1;

/** Access the DMAC instance using these structures when calling API functions directly (::p_api is not used). */
extern dmac_instance_ctrl_t g_transfer1_ctrl;
extern const transfer_cfg_t g_transfer1_cfg;

#ifndef NULL
void NULL(transfer_callback_args_t *p_args);
#endif
/* Transfer on DMAC Instance. */
extern const transfer_instance_t g_transfer0;

/** Access the DMAC instance using these structures when calling API functions directly (::p_api is not used). */
extern dmac_instance_ctrl_t g_transfer0_ctrl;
extern const transfer_cfg_t g_transfer0_cfg;

#ifndef usb_ip0_d1fifo_callback
void usb_ip0_d1fifo_callback(transfer_callback_args_t *p_args);
#endif
/* Basic on USB Instance. */
extern const usb_instance_t g_basic0;

/** Access the USB instance using these structures when calling API functions directly (::p_api is not used). */
extern usb_instance_ctrl_t g_basic0_ctrl;
extern const usb_cfg_t g_basic0_cfg;

#ifndef NULL
void NULL(void*);
#endif

#if 0 == BSP_CFG_RTOS
#ifndef usb_cdc_rtos_callback
void usb_cdc_rtos_callback(usb_callback_args_t*);
#endif
#endif

#if 2 == BSP_CFG_RTOS
#ifndef usb_cdc_rtos_callback
void usb_cdc_rtos_callback(usb_event_info_t *, usb_hdl_t, usb_onoff_t);
#endif
#endif
/** CDC Driver on USB Instance. */
FSP_FOOTER
#endif /* DOWNLOADER_THREAD_H_ */

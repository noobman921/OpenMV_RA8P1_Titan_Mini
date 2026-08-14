/* generated common header file - do not edit */
#ifndef COMMON_DATA_H_
#define COMMON_DATA_H_
#include <stdint.h>
#include "bsp_api.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "r_mram.h"
#include "r_flash_api.h"
#include "r_mipi_csi.h"
#include "r_mipi_csi_api.h"
#include "r_vin.h"
#include "r_capture_api.h"
#include "arm_math.h"
#include "r_ioport.h"
#include "bsp_pin_cfg.h"
FSP_HEADER
/* Flash on MRAM Instance */
extern const flash_instance_t g_mram0;

/** Access the MRAM instance using these structures when calling API functions directly (::p_api is not used). */
extern mram_instance_ctrl_t g_mram0_ctrl;
extern const flash_cfg_t g_mram0_cfg;

#ifndef NULL
void NULL(flash_callback_args_t *p_args);
#endif
/* MIPI PHY on MIPI PHY Instance. */

extern const mipi_phy_instance_t g_cam_mipi_phy;

/* Access the MIPI PHY instance using these structures when calling API functions directly (::p_api is not used). */
extern mipi_phy_ctrl_t g_cam_mipi_phy_ctrl;
extern const mipi_phy_cfg_t g_cam_mipi_phy_cfg;
/* MIPI CSI on MIPI CSI Instance. */
extern const mipi_csi_instance_t g_cam_mipi_csi;

/* Access the MIPI CSI instance using these structures when calling API functions directly (::p_api is not used). */
extern mipi_csi_instance_ctrl_t g_cam_mipi_csi_ctrl;
extern const mipi_csi_cfg_t g_cam_mipi_csi_cfg;

#ifndef cam_mipi_csi_callback
void cam_mipi_csi_callback(mipi_csi_callback_args_t *p_args);
#endif
/* MIPI VIN on MIPI VIN Instance. */
extern const capture_instance_t g_cam_vin;

/* Access the MIPI VIN instance using these structures when calling API functions directly (::p_api is not used). */
extern vin_instance_ctrl_t g_cam_vin_ctrl;
extern const capture_cfg_t g_cam_vin_cfg;

#ifndef cam_vin_callback
void cam_vin_callback(capture_callback_args_t *p_args);
#endif

#ifndef VIN_CFG_IMAGE_STRIDE
#define VIN_CFG_IMAGE_STRIDE (640)
#endif

#ifndef VIN_CFG_BYTES_PER_LINE
#define VIN_CFG_BYTES_PER_LINE (1280)
#endif

#define VIN_BYTES_PER_FRAME (VIN_CFG_BYTES_PER_LINE * 480)

extern uint8_t vin_image_buffer_1[VIN_BYTES_PER_FRAME];
extern uint8_t vin_image_buffer_2[VIN_BYTES_PER_FRAME];
extern uint8_t vin_image_buffer_3[VIN_BYTES_PER_FRAME];

#define IOPORT_CFG_NAME g_bsp_pin_cfg
#define IOPORT_CFG_OPEN R_IOPORT_Open
#define IOPORT_CFG_CTRL g_ioport_ctrl

/* IOPORT Instance */
extern const ioport_instance_t g_ioport;

/* IOPORT control structure. */
extern ioport_instance_ctrl_t g_ioport_ctrl;
extern SemaphoreHandle_t g_usb_write_complete_binary_semaphore;
extern QueueHandle_t g_usb_read_queue;
void g_common_init(void);
FSP_FOOTER
#endif /* COMMON_DATA_H_ */

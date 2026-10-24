/* generated HAL header file - do not edit */
#ifndef HAL_DATA_H_
#define HAL_DATA_H_
#include <stdint.h>
#include "bsp_api.h"
#include "common_data.h"
#include "r_gpt.h"
#include "r_timer_api.h"
#include "r_sci_b_uart.h"
#include "r_uart_api.h"
#include "r_spi_b.h"
#include "r_rtc.h"
#include "r_rtc_api.h"
#include "r_ospi_b.h"
#include "r_spi_flash_api.h"
#include "r_dmac.h"
#include "r_transfer_api.h"
#include "r_sdhi.h"
#include "r_sdmmc_api.h"
#include "r_usb_basic.h"
#include "r_usb_basic_api.h"
#include "r_usb_pcdc_api.h"
#include "r_iic_b_master.h"
#include "r_i2c_master_api.h"
#include "r_iic_master.h"
#include "r_i2c_master_api.h"
FSP_HEADER
/** Timer on GPT Instance. */
extern const timer_instance_t g_timer0;

/** Access the GPT instance using these structures when calling API functions directly (::p_api is not used). */
extern gpt_instance_ctrl_t g_timer0_ctrl;
extern const timer_cfg_t g_timer0_cfg;

#ifndef NULL
void NULL(timer_callback_args_t *p_args);
#endif
/** UART on SCI Instance. */
extern const uart_instance_t g_uart0;

/** Access the UART instance using these structures when calling API functions directly (::p_api is not used). */
extern sci_b_uart_instance_ctrl_t g_uart0_ctrl;
extern const uart_cfg_t g_uart0_cfg;
extern const sci_b_uart_extended_cfg_t g_uart0_cfg_extend;

#ifndef mpy_uart_callback
void mpy_uart_callback(uart_callback_args_t *p_args);
#endif
/** SPI on SPI Instance. */
extern const spi_instance_t g_spi0;

/** Access the SPI instance using these structures when calling API functions directly (::p_api is not used). */
extern spi_b_instance_ctrl_t g_spi0_ctrl;
extern const spi_cfg_t g_spi0_cfg;

/** Callback used by SPI Instance. */
#ifndef mpy_spi_callback
void mpy_spi_callback(spi_callback_args_t *p_args);
#endif

#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
#define g_spi0_P_TRANSFER_TX (NULL)
#else
    #define g_spi0_P_TRANSFER_TX (&RA_NOT_DEFINED)
#endif
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
#define g_spi0_P_TRANSFER_RX (NULL)
#else
    #define g_spi0_P_TRANSFER_RX (&RA_NOT_DEFINED)
#endif
#undef RA_NOT_DEFINED
/* RTC Instance. */
extern const rtc_instance_t g_rtc0;

/** Access the RTC instance using these structures when calling API functions directly (::p_api is not used). */
extern rtc_instance_ctrl_t g_rtc0_ctrl;
extern const rtc_cfg_t g_rtc0_cfg;

#ifndef NULL
void NULL(rtc_callback_args_t *p_args);
#endif
/** UART on SCI Instance. */
extern const uart_instance_t mpy_repl_uart;

/** Access the UART instance using these structures when calling API functions directly (::p_api is not used). */
extern sci_b_uart_instance_ctrl_t mpy_repl_uart_ctrl;
extern const uart_cfg_t mpy_repl_uart_cfg;
extern const sci_b_uart_extended_cfg_t mpy_repl_uart_cfg_extend;

#ifndef mpy_repl_uart_callback
void mpy_repl_uart_callback(uart_callback_args_t *p_args);
#endif
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
extern const transfer_instance_t g_transfer0;

/** Access the DMAC instance using these structures when calling API functions directly (::p_api is not used). */
extern dmac_instance_ctrl_t g_transfer0_ctrl;
extern const transfer_cfg_t g_transfer0_cfg;

#ifndef omv_sdmmc_dmac_callback
void omv_sdmmc_dmac_callback(transfer_callback_args_t *p_args);
#endif
/** SDMMC on SDMMC Instance. */
extern const sdmmc_instance_t omv_sdmmc;

/** Access the SDMMC instance using these structures when calling API functions directly (::p_api is not used). */
extern sdhi_instance_ctrl_t omv_sdmmc_ctrl;
extern sdmmc_cfg_t omv_sdmmc_cfg;

#ifndef omv_sdhi0_callback
void omv_sdhi0_callback(sdmmc_callback_args_t *p_args);
#endif
/* Transfer on DMAC Instance. */
extern const transfer_instance_t omv_csi_dmac_transfer;

/** Access the DMAC instance using these structures when calling API functions directly (::p_api is not used). */
extern dmac_instance_ctrl_t omv_csi_dmac_transfer_ctrl;
extern const transfer_cfg_t omv_csi_dmac_transfer_cfg;

#ifndef omv_csi_dmac_callback
void omv_csi_dmac_callback(transfer_callback_args_t *p_args);
#endif
/** Timer on GPT Instance. */
extern const timer_instance_t g_timer11;

/** Access the GPT instance using these structures when calling API functions directly (::p_api is not used). */
extern gpt_instance_ctrl_t g_timer11_ctrl;
extern const timer_cfg_t g_timer11_cfg;

#ifndef NULL
void NULL(timer_callback_args_t *p_args);
#endif
/* Basic on USB Instance. */
extern const usb_instance_t g_basic1;

/** Access the USB instance using these structures when calling API functions directly (::p_api is not used). */
extern usb_instance_ctrl_t g_basic1_ctrl;
extern const usb_cfg_t g_basic1_cfg;

#ifndef NULL
void NULL(void*);
#endif

#if 0 == BSP_CFG_RTOS
#ifndef NULL
void NULL(usb_callback_args_t*);
#endif
#endif

#if 2 == BSP_CFG_RTOS
#ifndef NULL
void NULL(usb_event_info_t *, usb_hdl_t, usb_onoff_t);
#endif
#endif
/** CDC Driver on USB Instance. */
/* I2C Master on IIC Instance. */
extern const i2c_master_instance_t g_i2c_master1;

/** Access the I2C Master instance using these structures when calling API functions directly (::p_api is not used). */
extern iic_b_master_instance_ctrl_t g_i2c_master1_ctrl;
extern const i2c_master_cfg_t g_i2c_master1_cfg;

#ifndef g_iic_b_master0_callback
void g_iic_b_master0_callback(i2c_master_callback_args_t *p_args);
#endif
/* I2C Master on IIC Instance. */
extern const i2c_master_instance_t omv_i2c;

/** Access the I2C Master instance using these structures when calling API functions directly (::p_api is not used). */
extern iic_master_instance_ctrl_t omv_i2c_ctrl;
extern const i2c_master_cfg_t omv_i2c_cfg;

#ifndef omv_i2c_callback
void omv_i2c_callback(i2c_master_callback_args_t *p_args);
#endif
void hal_entry(void);
void g_hal_init(void);
FSP_FOOTER
#endif /* HAL_DATA_H_ */

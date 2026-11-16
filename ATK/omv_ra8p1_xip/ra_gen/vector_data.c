/* generated vector source file - do not edit */
#include "bsp_api.h"
/* Do not build these data structures if no interrupts are currently allocated because IAR will have build errors. */
#if VECTOR_DATA_IRQ_COUNT > 0
        BSP_DONT_REMOVE const fsp_vector_t g_vector_table[BSP_ICU_VECTOR_NUM_ENTRIES] BSP_PLACE_IN_SECTION(BSP_SECTION_APPLICATION_VECTORS) =
        {
                        [0] = iic_b_master_rxi_isr, /* IICB0 RXI (Receive) */
            [1] = iic_b_master_txi_isr, /* IICB0 TXI (Transmit) */
            [2] = iic_b_master_tei_isr, /* IICB0 TEI (Transmit end) */
            [3] = iic_b_master_eri_isr, /* IICB0 ERI (Error) */
            [4] = usbfs_interrupt_handler, /* USBFS INT (USBFS interrupt) */
            [5] = usbfs_resume_handler, /* USBFS RESUME (USBFS resume interrupt) */
            [6] = usbfs_d0fifo_handler, /* USBFS FIFO 0 (DMA/DTC transfer request 0) */
            [7] = usbfs_d1fifo_handler, /* USBFS FIFO 1 (DMA/DTC transfer request 1) */
            [8] = usbhs_interrupt_handler, /* USBHS USB INT RESUME (USBHS interrupt) */
            [9] = usbhs_d0fifo_handler, /* USBHS FIFO 0 (DMA transfer request 0) */
            [10] = usbhs_d1fifo_handler, /* USBHS FIFO 1 (DMA transfer request 1) */
            [11] = vin_status_isr, /* VIN IRQ (Interrupt Request) */
            [12] = vin_error_isr, /* VIN ERR (Interrupt Request for SYNC Error) */
            [13] = mipi_csi_rx_isr, /* MIPICSI RX (Receive interrupt) */
            [14] = mipi_csi_dl_isr, /* MIPICSI DL (Data Lane interrupt) */
            [15] = mipi_csi_pm_isr, /* MIPICSI PM (Power Management interrupt) */
            [16] = mipi_csi_gst_isr, /* MIPICSI GST (Generic Short Packet interrupt) */
            [17] = dmac_int_isr, /* DMAC0 INT (DMAC0 transfer end) */
            [18] = sdhimmc_accs_isr, /* SDHIMMC0 ACCS (Card access) */
            [19] = sdhimmc_card_isr, /* SDHIMMC0 CARD (Card detect) */
            [20] = rm_ethosu_isr, /* NPU IRQ (NPU IRQ) */
            [21] = sci_b_uart_rxi_isr, /* SCI9 RXI (Receive data full) */
            [22] = sci_b_uart_txi_isr, /* SCI9 TXI (Transmit data empty) */
            [23] = sci_b_uart_tei_isr, /* SCI9 TEI (Transmit end) */
            [24] = sci_b_uart_eri_isr, /* SCI9 ERI (Receive error) */
            [25] = rtc_carry_isr, /* RTC CARRY (Carry interrupt) */
            [26] = spi_b_rxi_isr, /* SPI1 RXI (Receive buffer full) */
            [27] = spi_b_txi_isr, /* SPI1 TXI (Transmit buffer empty) */
            [28] = spi_b_tei_isr, /* SPI1 TEI (Transmission complete event) */
            [29] = spi_b_eri_isr, /* SPI1 ERI (Error) */
            [30] = sci_b_uart_rxi_isr, /* SCI2 RXI (Receive data full) */
            [31] = sci_b_uart_txi_isr, /* SCI2 TXI (Transmit data empty) */
            [32] = sci_b_uart_tei_isr, /* SCI2 TEI (Transmit end) */
            [33] = sci_b_uart_eri_isr, /* SCI2 ERI (Receive error) */
            [34] = sci_b_i2c_txi_isr, /* SCI0 TXI (Transmit data empty) */
            [35] = sci_b_i2c_tei_isr, /* SCI0 TEI (Transmit end) */
            [36] = ceu_isr, /* CEU CEUI (CEU interrupt) */
        };
        #if BSP_FEATURE_ICU_HAS_IELSR
        const bsp_interrupt_event_t g_interrupt_event_link_select[BSP_ICU_VECTOR_NUM_ENTRIES] =
        {
            [0] = BSP_PRV_VECT_ENUM(EVENT_IICB0_RXI,GROUP0), /* IICB0 RXI (Receive) */
            [1] = BSP_PRV_VECT_ENUM(EVENT_IICB0_TXI,GROUP1), /* IICB0 TXI (Transmit) */
            [2] = BSP_PRV_VECT_ENUM(EVENT_IICB0_TEI,GROUP2), /* IICB0 TEI (Transmit end) */
            [3] = BSP_PRV_VECT_ENUM(EVENT_IICB0_ERI,GROUP3), /* IICB0 ERI (Error) */
            [4] = BSP_PRV_VECT_ENUM(EVENT_USBFS_INT,GROUP4), /* USBFS INT (USBFS interrupt) */
            [5] = BSP_PRV_VECT_ENUM(EVENT_USBFS_RESUME,GROUP5), /* USBFS RESUME (USBFS resume interrupt) */
            [6] = BSP_PRV_VECT_ENUM(EVENT_USBFS_FIFO_0,GROUP6), /* USBFS FIFO 0 (DMA/DTC transfer request 0) */
            [7] = BSP_PRV_VECT_ENUM(EVENT_USBFS_FIFO_1,GROUP7), /* USBFS FIFO 1 (DMA/DTC transfer request 1) */
            [8] = BSP_PRV_VECT_ENUM(EVENT_USBHS_USB_INT_RESUME,GROUP0), /* USBHS USB INT RESUME (USBHS interrupt) */
            [9] = BSP_PRV_VECT_ENUM(EVENT_USBHS_FIFO_0,GROUP1), /* USBHS FIFO 0 (DMA transfer request 0) */
            [10] = BSP_PRV_VECT_ENUM(EVENT_USBHS_FIFO_1,GROUP2), /* USBHS FIFO 1 (DMA transfer request 1) */
            [11] = BSP_PRV_VECT_ENUM(EVENT_VIN_IRQ,GROUP3), /* VIN IRQ (Interrupt Request) */
            [12] = BSP_PRV_VECT_ENUM(EVENT_VIN_ERR,GROUP4), /* VIN ERR (Interrupt Request for SYNC Error) */
            [13] = BSP_PRV_VECT_ENUM(EVENT_MIPICSI_RX,GROUP5), /* MIPICSI RX (Receive interrupt) */
            [14] = BSP_PRV_VECT_ENUM(EVENT_MIPICSI_DL,GROUP6), /* MIPICSI DL (Data Lane interrupt) */
            [15] = BSP_PRV_VECT_ENUM(EVENT_MIPICSI_PM,GROUP7), /* MIPICSI PM (Power Management interrupt) */
            [16] = BSP_PRV_VECT_ENUM(EVENT_MIPICSI_GST,GROUP0), /* MIPICSI GST (Generic Short Packet interrupt) */
            [17] = BSP_PRV_VECT_ENUM(EVENT_DMAC0_INT,GROUP1), /* DMAC0 INT (DMAC0 transfer end) */
            [18] = BSP_PRV_VECT_ENUM(EVENT_SDHIMMC0_ACCS,GROUP2), /* SDHIMMC0 ACCS (Card access) */
            [19] = BSP_PRV_VECT_ENUM(EVENT_SDHIMMC0_CARD,GROUP3), /* SDHIMMC0 CARD (Card detect) */
            [20] = BSP_PRV_VECT_ENUM(EVENT_NPU_IRQ,GROUP4), /* NPU IRQ (NPU IRQ) */
            [21] = BSP_PRV_VECT_ENUM(EVENT_SCI9_RXI,GROUP5), /* SCI9 RXI (Receive data full) */
            [22] = BSP_PRV_VECT_ENUM(EVENT_SCI9_TXI,GROUP6), /* SCI9 TXI (Transmit data empty) */
            [23] = BSP_PRV_VECT_ENUM(EVENT_SCI9_TEI,GROUP7), /* SCI9 TEI (Transmit end) */
            [24] = BSP_PRV_VECT_ENUM(EVENT_SCI9_ERI,GROUP0), /* SCI9 ERI (Receive error) */
            [25] = BSP_PRV_VECT_ENUM(EVENT_RTC_CARRY,GROUP1), /* RTC CARRY (Carry interrupt) */
            [26] = BSP_PRV_VECT_ENUM(EVENT_SPI1_RXI,GROUP2), /* SPI1 RXI (Receive buffer full) */
            [27] = BSP_PRV_VECT_ENUM(EVENT_SPI1_TXI,GROUP3), /* SPI1 TXI (Transmit buffer empty) */
            [28] = BSP_PRV_VECT_ENUM(EVENT_SPI1_TEI,GROUP4), /* SPI1 TEI (Transmission complete event) */
            [29] = BSP_PRV_VECT_ENUM(EVENT_SPI1_ERI,GROUP5), /* SPI1 ERI (Error) */
            [30] = BSP_PRV_VECT_ENUM(EVENT_SCI2_RXI,GROUP6), /* SCI2 RXI (Receive data full) */
            [31] = BSP_PRV_VECT_ENUM(EVENT_SCI2_TXI,GROUP7), /* SCI2 TXI (Transmit data empty) */
            [32] = BSP_PRV_VECT_ENUM(EVENT_SCI2_TEI,FIXED), /* SCI2 TEI (Transmit end) */
            [33] = BSP_PRV_VECT_ENUM(EVENT_SCI2_ERI,FIXED), /* SCI2 ERI (Receive error) */
            [34] = BSP_PRV_VECT_ENUM(EVENT_SCI0_TXI,FIXED), /* SCI0 TXI (Transmit data empty) */
            [35] = BSP_PRV_VECT_ENUM(EVENT_SCI0_TEI,FIXED), /* SCI0 TEI (Transmit end) */
            [36] = BSP_PRV_VECT_ENUM(EVENT_CEU_CEUI,FIXED), /* CEU CEUI (CEU interrupt) */
        };
        #endif
        #endif

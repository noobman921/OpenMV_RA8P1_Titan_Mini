/* generated vector source file - do not edit */
#include "bsp_api.h"
/* Do not build these data structures if no interrupts are currently allocated because IAR will have build errors. */
#if VECTOR_DATA_IRQ_COUNT > 0
        BSP_DONT_REMOVE const fsp_vector_t g_vector_table[BSP_ICU_VECTOR_NUM_ENTRIES] BSP_PLACE_IN_SECTION(BSP_SECTION_APPLICATION_VECTORS) =
        {
                        [0] = iic_master_rxi_isr, /* IIC0 RXI (Receive data full) */
            [1] = iic_master_txi_isr, /* IIC0 TXI (Transmit data empty) */
            [2] = iic_master_tei_isr, /* IIC0 TEI (Transmit end) */
            [3] = iic_master_eri_isr, /* IIC0 ERI (Transfer error) */
            [4] = iic_b_master_rxi_isr, /* IICB0 RXI (Receive) */
            [5] = iic_b_master_txi_isr, /* IICB0 TXI (Transmit) */
            [6] = iic_b_master_tei_isr, /* IICB0 TEI (Transmit end) */
            [7] = iic_b_master_eri_isr, /* IICB0 ERI (Error) */
            [8] = usbfs_interrupt_handler, /* USBFS INT (USBFS interrupt) */
            [9] = usbfs_resume_handler, /* USBFS RESUME (USBFS resume interrupt) */
            [10] = usbfs_d0fifo_handler, /* USBFS FIFO 0 (DMA/DTC transfer request 0) */
            [11] = usbfs_d1fifo_handler, /* USBFS FIFO 1 (DMA/DTC transfer request 1) */
            [12] = usbhs_interrupt_handler, /* USBHS USB INT RESUME (USBHS interrupt) */
            [13] = usbhs_d0fifo_handler, /* USBHS FIFO 0 (DMA transfer request 0) */
            [14] = usbhs_d1fifo_handler, /* USBHS FIFO 1 (DMA transfer request 1) */
            [15] = vin_status_isr, /* VIN IRQ (Interrupt Request) */
            [16] = vin_error_isr, /* VIN ERR (Interrupt Request for SYNC Error) */
            [17] = mipi_csi_rx_isr, /* MIPICSI RX (Receive interrupt) */
            [18] = mipi_csi_dl_isr, /* MIPICSI DL (Data Lane interrupt) */
            [19] = mipi_csi_pm_isr, /* MIPICSI PM (Power Management interrupt) */
            [20] = mipi_csi_gst_isr, /* MIPICSI GST (Generic Short Packet interrupt) */
            [21] = dmac_int_isr, /* DMAC0 INT (DMAC0 transfer end) */
            [22] = sdhimmc_accs_isr, /* SDHIMMC0 ACCS (Card access) */
            [23] = sdhimmc_card_isr, /* SDHIMMC0 CARD (Card detect) */
            [24] = rm_ethosu_isr, /* NPU IRQ (NPU IRQ) */
            [25] = sci_b_uart_rxi_isr, /* SCI1 RXI (Receive data full) */
            [26] = sci_b_uart_txi_isr, /* SCI1 TXI (Transmit data empty) */
            [27] = sci_b_uart_tei_isr, /* SCI1 TEI (Transmit end) */
            [28] = sci_b_uart_eri_isr, /* SCI1 ERI (Receive error) */
            [29] = rtc_carry_isr, /* RTC CARRY (Carry interrupt) */
            [30] = spi_b_rxi_isr, /* SPI1 RXI (Receive buffer full) */
            [31] = spi_b_txi_isr, /* SPI1 TXI (Transmit buffer empty) */
            [32] = spi_b_tei_isr, /* SPI1 TEI (Transmission complete event) */
            [33] = spi_b_eri_isr, /* SPI1 ERI (Error) */
            [34] = sci_b_uart_rxi_isr, /* SCI2 RXI (Receive data full) */
            [35] = sci_b_uart_txi_isr, /* SCI2 TXI (Transmit data empty) */
            [36] = sci_b_uart_tei_isr, /* SCI2 TEI (Transmit end) */
            [37] = sci_b_uart_eri_isr, /* SCI2 ERI (Receive error) */
        };
        #if BSP_FEATURE_ICU_HAS_IELSR
        const bsp_interrupt_event_t g_interrupt_event_link_select[BSP_ICU_VECTOR_NUM_ENTRIES] =
        {
            [0] = BSP_PRV_VECT_ENUM(EVENT_IIC0_RXI,GROUP0), /* IIC0 RXI (Receive data full) */
            [1] = BSP_PRV_VECT_ENUM(EVENT_IIC0_TXI,GROUP1), /* IIC0 TXI (Transmit data empty) */
            [2] = BSP_PRV_VECT_ENUM(EVENT_IIC0_TEI,GROUP2), /* IIC0 TEI (Transmit end) */
            [3] = BSP_PRV_VECT_ENUM(EVENT_IIC0_ERI,GROUP3), /* IIC0 ERI (Transfer error) */
            [4] = BSP_PRV_VECT_ENUM(EVENT_IICB0_RXI,GROUP4), /* IICB0 RXI (Receive) */
            [5] = BSP_PRV_VECT_ENUM(EVENT_IICB0_TXI,GROUP5), /* IICB0 TXI (Transmit) */
            [6] = BSP_PRV_VECT_ENUM(EVENT_IICB0_TEI,GROUP6), /* IICB0 TEI (Transmit end) */
            [7] = BSP_PRV_VECT_ENUM(EVENT_IICB0_ERI,GROUP7), /* IICB0 ERI (Error) */
            [8] = BSP_PRV_VECT_ENUM(EVENT_USBFS_INT,GROUP0), /* USBFS INT (USBFS interrupt) */
            [9] = BSP_PRV_VECT_ENUM(EVENT_USBFS_RESUME,GROUP1), /* USBFS RESUME (USBFS resume interrupt) */
            [10] = BSP_PRV_VECT_ENUM(EVENT_USBFS_FIFO_0,GROUP2), /* USBFS FIFO 0 (DMA/DTC transfer request 0) */
            [11] = BSP_PRV_VECT_ENUM(EVENT_USBFS_FIFO_1,GROUP3), /* USBFS FIFO 1 (DMA/DTC transfer request 1) */
            [12] = BSP_PRV_VECT_ENUM(EVENT_USBHS_USB_INT_RESUME,GROUP4), /* USBHS USB INT RESUME (USBHS interrupt) */
            [13] = BSP_PRV_VECT_ENUM(EVENT_USBHS_FIFO_0,GROUP5), /* USBHS FIFO 0 (DMA transfer request 0) */
            [14] = BSP_PRV_VECT_ENUM(EVENT_USBHS_FIFO_1,GROUP6), /* USBHS FIFO 1 (DMA transfer request 1) */
            [15] = BSP_PRV_VECT_ENUM(EVENT_VIN_IRQ,GROUP7), /* VIN IRQ (Interrupt Request) */
            [16] = BSP_PRV_VECT_ENUM(EVENT_VIN_ERR,GROUP0), /* VIN ERR (Interrupt Request for SYNC Error) */
            [17] = BSP_PRV_VECT_ENUM(EVENT_MIPICSI_RX,GROUP1), /* MIPICSI RX (Receive interrupt) */
            [18] = BSP_PRV_VECT_ENUM(EVENT_MIPICSI_DL,GROUP2), /* MIPICSI DL (Data Lane interrupt) */
            [19] = BSP_PRV_VECT_ENUM(EVENT_MIPICSI_PM,GROUP3), /* MIPICSI PM (Power Management interrupt) */
            [20] = BSP_PRV_VECT_ENUM(EVENT_MIPICSI_GST,GROUP4), /* MIPICSI GST (Generic Short Packet interrupt) */
            [21] = BSP_PRV_VECT_ENUM(EVENT_DMAC0_INT,GROUP5), /* DMAC0 INT (DMAC0 transfer end) */
            [22] = BSP_PRV_VECT_ENUM(EVENT_SDHIMMC0_ACCS,GROUP6), /* SDHIMMC0 ACCS (Card access) */
            [23] = BSP_PRV_VECT_ENUM(EVENT_SDHIMMC0_CARD,GROUP7), /* SDHIMMC0 CARD (Card detect) */
            [24] = BSP_PRV_VECT_ENUM(EVENT_NPU_IRQ,GROUP0), /* NPU IRQ (NPU IRQ) */
            [25] = BSP_PRV_VECT_ENUM(EVENT_SCI1_RXI,GROUP1), /* SCI1 RXI (Receive data full) */
            [26] = BSP_PRV_VECT_ENUM(EVENT_SCI1_TXI,GROUP2), /* SCI1 TXI (Transmit data empty) */
            [27] = BSP_PRV_VECT_ENUM(EVENT_SCI1_TEI,GROUP3), /* SCI1 TEI (Transmit end) */
            [28] = BSP_PRV_VECT_ENUM(EVENT_SCI1_ERI,GROUP4), /* SCI1 ERI (Receive error) */
            [29] = BSP_PRV_VECT_ENUM(EVENT_RTC_CARRY,GROUP5), /* RTC CARRY (Carry interrupt) */
            [30] = BSP_PRV_VECT_ENUM(EVENT_SPI1_RXI,GROUP6), /* SPI1 RXI (Receive buffer full) */
            [31] = BSP_PRV_VECT_ENUM(EVENT_SPI1_TXI,GROUP7), /* SPI1 TXI (Transmit buffer empty) */
            [32] = BSP_PRV_VECT_ENUM(EVENT_SPI1_TEI,FIXED), /* SPI1 TEI (Transmission complete event) */
            [33] = BSP_PRV_VECT_ENUM(EVENT_SPI1_ERI,FIXED), /* SPI1 ERI (Error) */
            [34] = BSP_PRV_VECT_ENUM(EVENT_SCI2_RXI,FIXED), /* SCI2 RXI (Receive data full) */
            [35] = BSP_PRV_VECT_ENUM(EVENT_SCI2_TXI,FIXED), /* SCI2 TXI (Transmit data empty) */
            [36] = BSP_PRV_VECT_ENUM(EVENT_SCI2_TEI,FIXED), /* SCI2 TEI (Transmit end) */
            [37] = BSP_PRV_VECT_ENUM(EVENT_SCI2_ERI,FIXED), /* SCI2 ERI (Receive error) */
        };
        #endif
        #endif

/* generated vector source file - do not edit */
#include "bsp_api.h"
/* Do not build these data structures if no interrupts are currently allocated because IAR will have build errors. */
#if VECTOR_DATA_IRQ_COUNT > 0
        BSP_DONT_REMOVE const fsp_vector_t g_vector_table[BSP_ICU_VECTOR_NUM_ENTRIES] BSP_PLACE_IN_SECTION(BSP_SECTION_APPLICATION_VECTORS) =
        {
                        [4] = iic_master_rxi_isr, /* IIC0 RXI (Receive data full) */
            [5] = iic_master_txi_isr, /* IIC0 TXI (Transmit data empty) */
            [6] = iic_master_tei_isr, /* IIC0 TEI (Transmit end) */
            [7] = iic_master_eri_isr, /* IIC0 ERI (Transfer error) */
            [8] = iic_b_master_rxi_isr, /* IICB0 RXI (Receive) */
            [9] = iic_b_master_txi_isr, /* IICB0 TXI (Transmit) */
            [10] = iic_b_master_tei_isr, /* IICB0 TEI (Transmit end) */
            [11] = iic_b_master_eri_isr, /* IICB0 ERI (Error) */
            [12] = usbfs_interrupt_handler, /* USBFS INT (USBFS interrupt) */
            [13] = usbfs_resume_handler, /* USBFS RESUME (USBFS resume interrupt) */
            [14] = usbfs_d0fifo_handler, /* USBFS FIFO 0 (DMA/DTC transfer request 0) */
            [15] = usbfs_d1fifo_handler, /* USBFS FIFO 1 (DMA/DTC transfer request 1) */
            [16] = usbhs_interrupt_handler, /* USBHS USB INT RESUME (USBHS interrupt) */
            [17] = usbhs_d0fifo_handler, /* USBHS FIFO 0 (DMA transfer request 0) */
            [18] = usbhs_d1fifo_handler, /* USBHS FIFO 1 (DMA transfer request 1) */
            [19] = vin_status_isr, /* VIN IRQ (Interrupt Request) */
            [20] = vin_error_isr, /* VIN ERR (Interrupt Request for SYNC Error) */
            [21] = mipi_csi_rx_isr, /* MIPICSI RX (Receive interrupt) */
            [22] = mipi_csi_dl_isr, /* MIPICSI DL (Data Lane interrupt) */
            [23] = mipi_csi_vc_isr, /* MIPICSI VC (Virtual Channel interrupt) */
            [24] = mipi_csi_pm_isr, /* MIPICSI PM (Power Management interrupt) */
            [25] = mipi_csi_gst_isr, /* MIPICSI GST (Generic Short Packet interrupt) */
            [26] = dmac_int_isr, /* DMAC0 INT (DMAC0 transfer end) */
            [27] = sci_b_uart_rxi_isr, /* SCI1 RXI (Receive data full) */
            [28] = sci_b_uart_txi_isr, /* SCI1 TXI (Transmit data empty) */
            [29] = sci_b_uart_tei_isr, /* SCI1 TEI (Transmit end) */
            [30] = sci_b_uart_eri_isr, /* SCI1 ERI (Receive error) */
            [31] = rtc_carry_isr, /* RTC CARRY (Carry interrupt) */
            [32] = spi_b_rxi_isr, /* SPI1 RXI (Receive buffer full) */
            [33] = spi_b_txi_isr, /* SPI1 TXI (Transmit buffer empty) */
            [34] = spi_b_tei_isr, /* SPI1 TEI (Transmission complete event) */
            [35] = spi_b_eri_isr, /* SPI1 ERI (Error) */
            [36] = sci_b_uart_rxi_isr, /* SCI2 RXI (Receive data full) */
            [37] = sci_b_uart_txi_isr, /* SCI2 TXI (Transmit data empty) */
            [38] = sci_b_uart_tei_isr, /* SCI2 TEI (Transmit end) */
            [39] = sci_b_uart_eri_isr, /* SCI2 ERI (Receive error) */
        };
        #if BSP_FEATURE_ICU_HAS_IELSR
        const bsp_interrupt_event_t g_interrupt_event_link_select[BSP_ICU_VECTOR_NUM_ENTRIES] =
        {
            [4] = BSP_PRV_VECT_ENUM(EVENT_IIC0_RXI,GROUP4), /* IIC0 RXI (Receive data full) */
            [5] = BSP_PRV_VECT_ENUM(EVENT_IIC0_TXI,GROUP5), /* IIC0 TXI (Transmit data empty) */
            [6] = BSP_PRV_VECT_ENUM(EVENT_IIC0_TEI,GROUP6), /* IIC0 TEI (Transmit end) */
            [7] = BSP_PRV_VECT_ENUM(EVENT_IIC0_ERI,GROUP7), /* IIC0 ERI (Transfer error) */
            [8] = BSP_PRV_VECT_ENUM(EVENT_IICB0_RXI,GROUP0), /* IICB0 RXI (Receive) */
            [9] = BSP_PRV_VECT_ENUM(EVENT_IICB0_TXI,GROUP1), /* IICB0 TXI (Transmit) */
            [10] = BSP_PRV_VECT_ENUM(EVENT_IICB0_TEI,GROUP2), /* IICB0 TEI (Transmit end) */
            [11] = BSP_PRV_VECT_ENUM(EVENT_IICB0_ERI,GROUP3), /* IICB0 ERI (Error) */
            [12] = BSP_PRV_VECT_ENUM(EVENT_USBFS_INT,GROUP4), /* USBFS INT (USBFS interrupt) */
            [13] = BSP_PRV_VECT_ENUM(EVENT_USBFS_RESUME,GROUP5), /* USBFS RESUME (USBFS resume interrupt) */
            [14] = BSP_PRV_VECT_ENUM(EVENT_USBFS_FIFO_0,GROUP6), /* USBFS FIFO 0 (DMA/DTC transfer request 0) */
            [15] = BSP_PRV_VECT_ENUM(EVENT_USBFS_FIFO_1,GROUP7), /* USBFS FIFO 1 (DMA/DTC transfer request 1) */
            [16] = BSP_PRV_VECT_ENUM(EVENT_USBHS_USB_INT_RESUME,GROUP0), /* USBHS USB INT RESUME (USBHS interrupt) */
            [17] = BSP_PRV_VECT_ENUM(EVENT_USBHS_FIFO_0,GROUP1), /* USBHS FIFO 0 (DMA transfer request 0) */
            [18] = BSP_PRV_VECT_ENUM(EVENT_USBHS_FIFO_1,GROUP2), /* USBHS FIFO 1 (DMA transfer request 1) */
            [19] = BSP_PRV_VECT_ENUM(EVENT_VIN_IRQ,GROUP3), /* VIN IRQ (Interrupt Request) */
            [20] = BSP_PRV_VECT_ENUM(EVENT_VIN_ERR,GROUP4), /* VIN ERR (Interrupt Request for SYNC Error) */
            [21] = BSP_PRV_VECT_ENUM(EVENT_MIPICSI_RX,GROUP5), /* MIPICSI RX (Receive interrupt) */
            [22] = BSP_PRV_VECT_ENUM(EVENT_MIPICSI_DL,GROUP6), /* MIPICSI DL (Data Lane interrupt) */
            [23] = BSP_PRV_VECT_ENUM(EVENT_MIPICSI_VC,GROUP7), /* MIPICSI VC (Virtual Channel interrupt) */
            [24] = BSP_PRV_VECT_ENUM(EVENT_MIPICSI_PM,GROUP0), /* MIPICSI PM (Power Management interrupt) */
            [25] = BSP_PRV_VECT_ENUM(EVENT_MIPICSI_GST,GROUP1), /* MIPICSI GST (Generic Short Packet interrupt) */
            [26] = BSP_PRV_VECT_ENUM(EVENT_DMAC0_INT,GROUP2), /* DMAC0 INT (DMAC0 transfer end) */
            [27] = BSP_PRV_VECT_ENUM(EVENT_SCI1_RXI,GROUP3), /* SCI1 RXI (Receive data full) */
            [28] = BSP_PRV_VECT_ENUM(EVENT_SCI1_TXI,GROUP4), /* SCI1 TXI (Transmit data empty) */
            [29] = BSP_PRV_VECT_ENUM(EVENT_SCI1_TEI,GROUP5), /* SCI1 TEI (Transmit end) */
            [30] = BSP_PRV_VECT_ENUM(EVENT_SCI1_ERI,GROUP6), /* SCI1 ERI (Receive error) */
            [31] = BSP_PRV_VECT_ENUM(EVENT_RTC_CARRY,GROUP7), /* RTC CARRY (Carry interrupt) */
            [32] = BSP_PRV_VECT_ENUM(EVENT_SPI1_RXI,FIXED), /* SPI1 RXI (Receive buffer full) */
            [33] = BSP_PRV_VECT_ENUM(EVENT_SPI1_TXI,FIXED), /* SPI1 TXI (Transmit buffer empty) */
            [34] = BSP_PRV_VECT_ENUM(EVENT_SPI1_TEI,FIXED), /* SPI1 TEI (Transmission complete event) */
            [35] = BSP_PRV_VECT_ENUM(EVENT_SPI1_ERI,FIXED), /* SPI1 ERI (Error) */
            [36] = BSP_PRV_VECT_ENUM(EVENT_SCI2_RXI,FIXED), /* SCI2 RXI (Receive data full) */
            [37] = BSP_PRV_VECT_ENUM(EVENT_SCI2_TXI,FIXED), /* SCI2 TXI (Transmit data empty) */
            [38] = BSP_PRV_VECT_ENUM(EVENT_SCI2_TEI,FIXED), /* SCI2 TEI (Transmit end) */
            [39] = BSP_PRV_VECT_ENUM(EVENT_SCI2_ERI,FIXED), /* SCI2 ERI (Receive error) */
        };
        #endif
        #endif

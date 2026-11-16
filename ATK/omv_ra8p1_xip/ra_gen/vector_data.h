/* generated vector header file - do not edit */
#ifndef VECTOR_DATA_H
#define VECTOR_DATA_H
#ifdef __cplusplus
        extern "C" {
        #endif
/* Number of interrupts allocated */
#ifndef VECTOR_DATA_IRQ_COUNT
#define VECTOR_DATA_IRQ_COUNT    (37)
#endif
/* ISR prototypes */
void iic_b_master_rxi_isr(void);
void iic_b_master_txi_isr(void);
void iic_b_master_tei_isr(void);
void iic_b_master_eri_isr(void);
void usbfs_interrupt_handler(void);
void usbfs_resume_handler(void);
void usbfs_d0fifo_handler(void);
void usbfs_d1fifo_handler(void);
void usbhs_interrupt_handler(void);
void usbhs_d0fifo_handler(void);
void usbhs_d1fifo_handler(void);
void vin_status_isr(void);
void vin_error_isr(void);
void mipi_csi_rx_isr(void);
void mipi_csi_dl_isr(void);
void mipi_csi_pm_isr(void);
void mipi_csi_gst_isr(void);
void dmac_int_isr(void);
void sdhimmc_accs_isr(void);
void sdhimmc_card_isr(void);
void rm_ethosu_isr(void);
void sci_b_uart_rxi_isr(void);
void sci_b_uart_txi_isr(void);
void sci_b_uart_tei_isr(void);
void sci_b_uart_eri_isr(void);
void rtc_carry_isr(void);
void spi_b_rxi_isr(void);
void spi_b_txi_isr(void);
void spi_b_tei_isr(void);
void spi_b_eri_isr(void);
void sci_b_i2c_txi_isr(void);
void sci_b_i2c_tei_isr(void);
void ceu_isr(void);

/* Vector table allocations */
#define VECTOR_NUMBER_IICB0_RXI ((IRQn_Type) 0) /* IICB0 RXI (Receive) */
#define IICB0_RXI_IRQn          ((IRQn_Type) 0) /* IICB0 RXI (Receive) */
#define VECTOR_NUMBER_IICB0_TXI ((IRQn_Type) 1) /* IICB0 TXI (Transmit) */
#define IICB0_TXI_IRQn          ((IRQn_Type) 1) /* IICB0 TXI (Transmit) */
#define VECTOR_NUMBER_IICB0_TEI ((IRQn_Type) 2) /* IICB0 TEI (Transmit end) */
#define IICB0_TEI_IRQn          ((IRQn_Type) 2) /* IICB0 TEI (Transmit end) */
#define VECTOR_NUMBER_IICB0_ERI ((IRQn_Type) 3) /* IICB0 ERI (Error) */
#define IICB0_ERI_IRQn          ((IRQn_Type) 3) /* IICB0 ERI (Error) */
#define VECTOR_NUMBER_USBFS_INT ((IRQn_Type) 4) /* USBFS INT (USBFS interrupt) */
#define USBFS_INT_IRQn          ((IRQn_Type) 4) /* USBFS INT (USBFS interrupt) */
#define VECTOR_NUMBER_USBFS_RESUME ((IRQn_Type) 5) /* USBFS RESUME (USBFS resume interrupt) */
#define USBFS_RESUME_IRQn          ((IRQn_Type) 5) /* USBFS RESUME (USBFS resume interrupt) */
#define VECTOR_NUMBER_USBFS_FIFO_0 ((IRQn_Type) 6) /* USBFS FIFO 0 (DMA/DTC transfer request 0) */
#define USBFS_FIFO_0_IRQn          ((IRQn_Type) 6) /* USBFS FIFO 0 (DMA/DTC transfer request 0) */
#define VECTOR_NUMBER_USBFS_FIFO_1 ((IRQn_Type) 7) /* USBFS FIFO 1 (DMA/DTC transfer request 1) */
#define USBFS_FIFO_1_IRQn          ((IRQn_Type) 7) /* USBFS FIFO 1 (DMA/DTC transfer request 1) */
#define VECTOR_NUMBER_USBHS_USB_INT_RESUME ((IRQn_Type) 8) /* USBHS USB INT RESUME (USBHS interrupt) */
#define USBHS_USB_INT_RESUME_IRQn          ((IRQn_Type) 8) /* USBHS USB INT RESUME (USBHS interrupt) */
#define VECTOR_NUMBER_USBHS_FIFO_0 ((IRQn_Type) 9) /* USBHS FIFO 0 (DMA transfer request 0) */
#define USBHS_FIFO_0_IRQn          ((IRQn_Type) 9) /* USBHS FIFO 0 (DMA transfer request 0) */
#define VECTOR_NUMBER_USBHS_FIFO_1 ((IRQn_Type) 10) /* USBHS FIFO 1 (DMA transfer request 1) */
#define USBHS_FIFO_1_IRQn          ((IRQn_Type) 10) /* USBHS FIFO 1 (DMA transfer request 1) */
#define VECTOR_NUMBER_VIN_IRQ ((IRQn_Type) 11) /* VIN IRQ (Interrupt Request) */
#define VIN_IRQ_IRQn          ((IRQn_Type) 11) /* VIN IRQ (Interrupt Request) */
#define VECTOR_NUMBER_VIN_ERR ((IRQn_Type) 12) /* VIN ERR (Interrupt Request for SYNC Error) */
#define VIN_ERR_IRQn          ((IRQn_Type) 12) /* VIN ERR (Interrupt Request for SYNC Error) */
#define VECTOR_NUMBER_MIPICSI_RX ((IRQn_Type) 13) /* MIPICSI RX (Receive interrupt) */
#define MIPICSI_RX_IRQn          ((IRQn_Type) 13) /* MIPICSI RX (Receive interrupt) */
#define VECTOR_NUMBER_MIPICSI_DL ((IRQn_Type) 14) /* MIPICSI DL (Data Lane interrupt) */
#define MIPICSI_DL_IRQn          ((IRQn_Type) 14) /* MIPICSI DL (Data Lane interrupt) */
#define VECTOR_NUMBER_MIPICSI_PM ((IRQn_Type) 15) /* MIPICSI PM (Power Management interrupt) */
#define MIPICSI_PM_IRQn          ((IRQn_Type) 15) /* MIPICSI PM (Power Management interrupt) */
#define VECTOR_NUMBER_MIPICSI_GST ((IRQn_Type) 16) /* MIPICSI GST (Generic Short Packet interrupt) */
#define MIPICSI_GST_IRQn          ((IRQn_Type) 16) /* MIPICSI GST (Generic Short Packet interrupt) */
#define VECTOR_NUMBER_DMAC0_INT ((IRQn_Type) 17) /* DMAC0 INT (DMAC0 transfer end) */
#define DMAC0_INT_IRQn          ((IRQn_Type) 17) /* DMAC0 INT (DMAC0 transfer end) */
#define VECTOR_NUMBER_SDHIMMC0_ACCS ((IRQn_Type) 18) /* SDHIMMC0 ACCS (Card access) */
#define SDHIMMC0_ACCS_IRQn          ((IRQn_Type) 18) /* SDHIMMC0 ACCS (Card access) */
#define VECTOR_NUMBER_SDHIMMC0_CARD ((IRQn_Type) 19) /* SDHIMMC0 CARD (Card detect) */
#define SDHIMMC0_CARD_IRQn          ((IRQn_Type) 19) /* SDHIMMC0 CARD (Card detect) */
#define VECTOR_NUMBER_NPU_IRQ ((IRQn_Type) 20) /* NPU IRQ (NPU IRQ) */
#define NPU_IRQ_IRQn          ((IRQn_Type) 20) /* NPU IRQ (NPU IRQ) */
#define VECTOR_NUMBER_SCI9_RXI ((IRQn_Type) 21) /* SCI9 RXI (Receive data full) */
#define SCI9_RXI_IRQn          ((IRQn_Type) 21) /* SCI9 RXI (Receive data full) */
#define VECTOR_NUMBER_SCI9_TXI ((IRQn_Type) 22) /* SCI9 TXI (Transmit data empty) */
#define SCI9_TXI_IRQn          ((IRQn_Type) 22) /* SCI9 TXI (Transmit data empty) */
#define VECTOR_NUMBER_SCI9_TEI ((IRQn_Type) 23) /* SCI9 TEI (Transmit end) */
#define SCI9_TEI_IRQn          ((IRQn_Type) 23) /* SCI9 TEI (Transmit end) */
#define VECTOR_NUMBER_SCI9_ERI ((IRQn_Type) 24) /* SCI9 ERI (Receive error) */
#define SCI9_ERI_IRQn          ((IRQn_Type) 24) /* SCI9 ERI (Receive error) */
#define VECTOR_NUMBER_RTC_CARRY ((IRQn_Type) 25) /* RTC CARRY (Carry interrupt) */
#define RTC_CARRY_IRQn          ((IRQn_Type) 25) /* RTC CARRY (Carry interrupt) */
#define VECTOR_NUMBER_SPI1_RXI ((IRQn_Type) 26) /* SPI1 RXI (Receive buffer full) */
#define SPI1_RXI_IRQn          ((IRQn_Type) 26) /* SPI1 RXI (Receive buffer full) */
#define VECTOR_NUMBER_SPI1_TXI ((IRQn_Type) 27) /* SPI1 TXI (Transmit buffer empty) */
#define SPI1_TXI_IRQn          ((IRQn_Type) 27) /* SPI1 TXI (Transmit buffer empty) */
#define VECTOR_NUMBER_SPI1_TEI ((IRQn_Type) 28) /* SPI1 TEI (Transmission complete event) */
#define SPI1_TEI_IRQn          ((IRQn_Type) 28) /* SPI1 TEI (Transmission complete event) */
#define VECTOR_NUMBER_SPI1_ERI ((IRQn_Type) 29) /* SPI1 ERI (Error) */
#define SPI1_ERI_IRQn          ((IRQn_Type) 29) /* SPI1 ERI (Error) */
#define VECTOR_NUMBER_SCI2_RXI ((IRQn_Type) 30) /* SCI2 RXI (Receive data full) */
#define SCI2_RXI_IRQn          ((IRQn_Type) 30) /* SCI2 RXI (Receive data full) */
#define VECTOR_NUMBER_SCI2_TXI ((IRQn_Type) 31) /* SCI2 TXI (Transmit data empty) */
#define SCI2_TXI_IRQn          ((IRQn_Type) 31) /* SCI2 TXI (Transmit data empty) */
#define VECTOR_NUMBER_SCI2_TEI ((IRQn_Type) 32) /* SCI2 TEI (Transmit end) */
#define SCI2_TEI_IRQn          ((IRQn_Type) 32) /* SCI2 TEI (Transmit end) */
#define VECTOR_NUMBER_SCI2_ERI ((IRQn_Type) 33) /* SCI2 ERI (Receive error) */
#define SCI2_ERI_IRQn          ((IRQn_Type) 33) /* SCI2 ERI (Receive error) */
#define VECTOR_NUMBER_SCI0_TXI ((IRQn_Type) 34) /* SCI0 TXI (Transmit data empty) */
#define SCI0_TXI_IRQn          ((IRQn_Type) 34) /* SCI0 TXI (Transmit data empty) */
#define VECTOR_NUMBER_SCI0_TEI ((IRQn_Type) 35) /* SCI0 TEI (Transmit end) */
#define SCI0_TEI_IRQn          ((IRQn_Type) 35) /* SCI0 TEI (Transmit end) */
#define VECTOR_NUMBER_CEU_CEUI ((IRQn_Type) 36) /* CEU CEUI (CEU interrupt) */
#define CEU_CEUI_IRQn          ((IRQn_Type) 36) /* CEU CEUI (CEU interrupt) */
/* The number of entries required for the ICU vector table. */
#define BSP_ICU_VECTOR_NUM_ENTRIES (37)

#ifdef __cplusplus
        }
        #endif
#endif /* VECTOR_DATA_H */

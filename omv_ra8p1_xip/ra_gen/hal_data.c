/* generated HAL source file - do not edit */
#include "hal_data.h"

gpt_instance_ctrl_t g_timer0_ctrl;
#if 0
const gpt_extended_pwm_cfg_t g_timer0_pwm_extend =
{
    .trough_ipl             = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT0_COUNTER_UNDERFLOW)
    .trough_irq             = VECTOR_NUMBER_GPT0_COUNTER_UNDERFLOW,
#else
    .trough_irq             = FSP_INVALID_VECTOR,
#endif
    .poeg_link              = GPT_POEG_LINK_POEG0,
    .output_disable         = (gpt_output_disable_t) ( GPT_OUTPUT_DISABLE_NONE),
    .adc_trigger            = (gpt_adc_trigger_t) ( GPT_ADC_TRIGGER_NONE),
    .dead_time_count_up     = 0,
    .dead_time_count_down   = 0,
    .adc_a_compare_match    = 0,
    .adc_b_compare_match    = 0,
    .interrupt_skip_source  = GPT_INTERRUPT_SKIP_SOURCE_NONE,
    .interrupt_skip_count   = GPT_INTERRUPT_SKIP_COUNT_0,
    .interrupt_skip_adc     = GPT_INTERRUPT_SKIP_ADC_NONE,
    .gtioca_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
    .gtiocb_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
};
#endif
const gpt_extended_cfg_t g_timer0_extend =
        { .gtioca =
        { .output_enabled = true, .stop_level = GPT_PIN_LEVEL_LOW },
          .gtiocb =
          { .output_enabled = false, .stop_level = GPT_PIN_LEVEL_LOW },
          .start_source = (gpt_source_t) (GPT_SOURCE_NONE), .stop_source = (gpt_source_t) (GPT_SOURCE_NONE), .clear_source =
                  (gpt_source_t) (GPT_SOURCE_NONE),
          .count_up_source = (gpt_source_t) (GPT_SOURCE_NONE), .count_down_source = (gpt_source_t) (GPT_SOURCE_NONE), .capture_a_source =
                  (gpt_source_t) (GPT_SOURCE_NONE),
          .capture_b_source = (gpt_source_t) (GPT_SOURCE_NONE), .capture_a_ipl = (BSP_IRQ_DISABLED), .capture_b_ipl =
                  (BSP_IRQ_DISABLED),
          .compare_match_c_ipl = (BSP_IRQ_DISABLED), .compare_match_d_ipl = (BSP_IRQ_DISABLED), .compare_match_e_ipl =
                  (BSP_IRQ_DISABLED),
          .compare_match_f_ipl = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT0_CAPTURE_COMPARE_A)
    .capture_a_irq         = VECTOR_NUMBER_GPT0_CAPTURE_COMPARE_A,
#else
          .capture_a_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT0_CAPTURE_COMPARE_B)
    .capture_b_irq         = VECTOR_NUMBER_GPT0_CAPTURE_COMPARE_B,
#else
          .capture_b_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT0_COMPARE_C)
    .compare_match_c_irq   = VECTOR_NUMBER_GPT0_COMPARE_C,
#else
          .compare_match_c_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT0_COMPARE_D)
    .compare_match_d_irq   = VECTOR_NUMBER_GPT0_COMPARE_D,
#else
          .compare_match_d_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT0_COMPARE_E)
    .compare_match_e_irq   = VECTOR_NUMBER_GPT0_COMPARE_E,
#else
          .compare_match_e_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT0_COMPARE_F)
    .compare_match_f_irq   = VECTOR_NUMBER_GPT0_COMPARE_F,
#else
          .compare_match_f_irq = FSP_INVALID_VECTOR,
#endif
          .compare_match_value =
          { (uint32_t) 0x0, /* CMP_A */
            (uint32_t) 0x0, /* CMP_B */
            (uint32_t) 0x0, /* CMP_C */
            (uint32_t) 0x0, /* CMP_D */
            (uint32_t) 0x0, /* CMP_E */
            (uint32_t) 0x0, /* CMP_F */},
          .compare_match_status = ((0U << 5U) | (0U << 4U) | (0U << 3U) | (0U << 2U) | (0U << 1U) | 0U), .capture_filter_gtioca =
                  GPT_CAPTURE_FILTER_NONE,
          .capture_filter_gtiocb = GPT_CAPTURE_FILTER_NONE,
#if 0
    .p_pwm_cfg             = &g_timer0_pwm_extend,
#else
          .p_pwm_cfg = NULL,
#endif
#if 0
    .gtior_setting.gtior_b.gtioa  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.oadflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.oahld  = 0U,
    .gtior_setting.gtior_b.oae    = (uint32_t) true,
    .gtior_setting.gtior_b.oadf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfaen  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsa  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
    .gtior_setting.gtior_b.gtiob  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.obdflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.obhld  = 0U,
    .gtior_setting.gtior_b.obe    = (uint32_t) false,
    .gtior_setting.gtior_b.obdf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfben  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsb  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
#else
          .gtior_setting.gtior = 0U,
#endif

          .gtioca_polarity = GPT_GTIOC_POLARITY_NORMAL,
          .gtiocb_polarity = GPT_GTIOC_POLARITY_NORMAL, };

const timer_cfg_t g_timer0_cfg =
{ .mode = TIMER_MODE_PWM,
/* Actual period: 0.00005 seconds. Actual duty: 50%. */.period_counts = (uint32_t) 0x30d4,
  .duty_cycle_counts = 0x186a, .source_div = (timer_source_div_t) 0, .channel = 0, .p_callback = NULL,
  /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
  .p_context = (void*) &NULL,
#endif
  .p_extend = &g_timer0_extend,
  .cycle_end_ipl = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT0_COUNTER_OVERFLOW)
    .cycle_end_irq       = VECTOR_NUMBER_GPT0_COUNTER_OVERFLOW,
#else
  .cycle_end_irq = FSP_INVALID_VECTOR,
#endif
        };
/* Instance structure to use this module. */
const timer_instance_t g_timer0 =
{ .p_ctrl = &g_timer0_ctrl, .p_cfg = &g_timer0_cfg, .p_api = &g_timer_on_gpt };
sci_b_uart_instance_ctrl_t g_uart0_ctrl;

sci_b_baud_setting_t g_uart0_baud_setting =
        {
        /* Baud rate calculated with 0.469% error. */.baudrate_bits_b.abcse = 0,
          .baudrate_bits_b.abcs = 0, .baudrate_bits_b.bgdm = 1, .baudrate_bits_b.cks = 0, .baudrate_bits_b.brr = 53, .baudrate_bits_b.mddr =
                  (uint8_t) 256,
          .baudrate_bits_b.brme = false };

/** UART extended configuration for UARTonSCI HAL driver */
const sci_b_uart_extended_cfg_t g_uart0_cfg_extend =
{ .clock = SCI_B_UART_CLOCK_INT, .rx_edge_start = SCI_B_UART_START_BIT_FALLING_EDGE, .noise_cancel =
          SCI_B_UART_NOISE_CANCELLATION_DISABLE,
  .rx_fifo_trigger = SCI_B_UART_RX_FIFO_TRIGGER_MAX, .p_baud_setting = &g_uart0_baud_setting, .flow_control =
          SCI_B_UART_FLOW_CONTROL_RTS,
#if 0xFF != 0xFF
                .flow_control_pin       = BSP_IO_PORT_FF_PIN_0xFF,
                #else
  .flow_control_pin = (bsp_io_port_pin_t) UINT16_MAX,
#endif
  .rs485_setting =
  { .enable = SCI_B_UART_RS485_DISABLE,
    .polarity = SCI_B_UART_RS485_DE_POLARITY_HIGH,
    .assertion_time = 1,
    .negation_time = 1, },
  .delay_cycles = 0, };

/** UART interface configuration */
const uart_cfg_t g_uart0_cfg =
{ .channel = 2, .data_bits = UART_DATA_BITS_8, .parity = UART_PARITY_OFF, .stop_bits = UART_STOP_BITS_1, .p_callback =
          mpy_uart_callback,
  .p_context = NULL, .p_extend = &g_uart0_cfg_extend,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
  .p_transfer_tx = NULL,
#else
                .p_transfer_tx       = &RA_NOT_DEFINED,
#endif
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
  .p_transfer_rx = NULL,
#else
                .p_transfer_rx       = &RA_NOT_DEFINED,
#endif
#undef RA_NOT_DEFINED
  .rxi_ipl = (12),
  .txi_ipl = (12), .tei_ipl = (12), .eri_ipl = (12),
#if defined(VECTOR_NUMBER_SCI2_RXI)
                .rxi_irq             = VECTOR_NUMBER_SCI2_RXI,
#else
  .rxi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI2_TXI)
                .txi_irq             = VECTOR_NUMBER_SCI2_TXI,
#else
  .txi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI2_TEI)
                .tei_irq             = VECTOR_NUMBER_SCI2_TEI,
#else
  .tei_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI2_ERI)
                .eri_irq             = VECTOR_NUMBER_SCI2_ERI,
#else
  .eri_irq = FSP_INVALID_VECTOR,
#endif
        };

/* Instance structure to use this module. */
const uart_instance_t g_uart0 =
{ .p_ctrl = &g_uart0_ctrl, .p_cfg = &g_uart0_cfg, .p_api = &g_uart_on_sci_b };
#define RA_NOT_DEFINED (UINT32_MAX)
#if (RA_NOT_DEFINED) != (RA_NOT_DEFINED)

/* If the transfer module is DMAC, define a DMAC transfer callback. */
#include "r_dmac.h"
extern void spi_b_tx_dmac_callback(spi_b_instance_ctrl_t const * const p_ctrl);

void g_spi0_tx_transfer_callback (dmac_callback_args_t * p_args)
{
    FSP_PARAMETER_NOT_USED(p_args);
    spi_b_tx_dmac_callback(&g_spi0_ctrl);
}
#endif

#if (RA_NOT_DEFINED) != (RA_NOT_DEFINED)

/* If the transfer module is DMAC, define a DMAC transfer callback. */
#include "r_dmac.h"
extern void spi_b_rx_dmac_callback(spi_b_instance_ctrl_t const * const p_ctrl);

void g_spi0_rx_transfer_callback (dmac_callback_args_t * p_args)
{
    FSP_PARAMETER_NOT_USED(p_args);
    spi_b_rx_dmac_callback(&g_spi0_ctrl);
}
#endif
#undef RA_NOT_DEFINED

spi_b_instance_ctrl_t g_spi0_ctrl;

/** SPI extended configuration for SPI HAL driver */
const spi_b_extended_cfg_t g_spi0_ext_cfg =
{ .spi_clksyn = SPI_B_SSL_MODE_SPI,
  .spi_comm = SPI_B_COMMUNICATION_FULL_DUPLEX,
  .ssl_polarity = SPI_B_SSLP_LOW,
  .ssl_select = SPI_B_SSL_SELECT_SSL0,
  .mosi_idle = SPI_B_MOSI_IDLE_VALUE_FIXING_DISABLE,
  .parity = SPI_B_PARITY_MODE_DISABLE,
  .byte_swap = SPI_B_BYTE_SWAP_DISABLE,
  .clock_source = SPI_B_CLOCK_SOURCE_PCLK,
  .spck_div =
  {
  /* Actual calculated bitrate: 15625000. */.spbr = 3,
    .brdv = 0 },
  .spck_delay = SPI_B_DELAY_COUNT_1,
  .ssl_negation_delay = SPI_B_DELAY_COUNT_1,
  .next_access_delay = SPI_B_DELAY_COUNT_1,
  .burst_interframe_delay = SPI_B_BURST_TRANSFER_WITH_DELAY

};

/** SPI configuration for SPI HAL driver */
const spi_cfg_t g_spi0_cfg =
{ .channel = 1,

#if defined(VECTOR_NUMBER_SPI1_RXI)
    .rxi_irq             = VECTOR_NUMBER_SPI1_RXI,
#else
  .rxi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SPI1_TXI)
    .txi_irq             = VECTOR_NUMBER_SPI1_TXI,
#else
  .txi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SPI1_TEI)
    .tei_irq             = VECTOR_NUMBER_SPI1_TEI,
#else
  .tei_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SPI1_ERI)
    .eri_irq             = VECTOR_NUMBER_SPI1_ERI,
#else
  .eri_irq = FSP_INVALID_VECTOR,
#endif

  .rxi_ipl = (12),
  .txi_ipl = (12),
  .tei_ipl = (12),
  .eri_ipl = (12),

  .operating_mode = SPI_MODE_MASTER,

  .clk_phase = SPI_CLK_PHASE_EDGE_ODD,
  .clk_polarity = SPI_CLK_POLARITY_LOW,

  .mode_fault = SPI_MODE_FAULT_ERROR_DISABLE,
  .bit_order = SPI_BIT_ORDER_MSB_FIRST,
  .p_transfer_tx = g_spi0_P_TRANSFER_TX,
  .p_transfer_rx = g_spi0_P_TRANSFER_RX,
  .p_callback = mpy_spi_callback,

  .p_context = NULL,
  .p_extend = (void*) &g_spi0_ext_cfg, };

/* Instance structure to use this module. */
const spi_instance_t g_spi0 =
{ .p_ctrl = &g_spi0_ctrl, .p_cfg = &g_spi0_cfg, .p_api = &g_spi_on_spi_b };
rtc_instance_ctrl_t g_rtc0_ctrl;
const rtc_error_adjustment_cfg_t g_rtc0_err_cfg =
{ .adjustment_mode = RTC_ERROR_ADJUSTMENT_MODE_AUTOMATIC,
  .adjustment_period = RTC_ERROR_ADJUSTMENT_PERIOD_10_SECOND,
  .adjustment_type = RTC_ERROR_ADJUSTMENT_NONE,
  .adjustment_value = 0, };
const rtc_cfg_t g_rtc0_cfg =
{ .clock_source = RTC_CLOCK_SOURCE_LOCO,
  .freq_compare_value = 255,
  .p_err_cfg = &g_rtc0_err_cfg,
  .p_callback = NULL,
  .p_context = NULL,
  .p_extend = NULL,
  .alarm_ipl = (BSP_IRQ_DISABLED),
  .periodic_ipl = (BSP_IRQ_DISABLED),
  .carry_ipl = (12),
#if defined(VECTOR_NUMBER_RTC_ALARM)
    .alarm_irq               = VECTOR_NUMBER_RTC_ALARM,
#else
  .alarm_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_RTC_PERIOD)
    .periodic_irq            = VECTOR_NUMBER_RTC_PERIOD,
#else
  .periodic_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_RTC_CARRY)
    .carry_irq               = VECTOR_NUMBER_RTC_CARRY,
#else
  .carry_irq = FSP_INVALID_VECTOR,
#endif
        };
/* Instance structure to use this module. */
const rtc_instance_t g_rtc0 =
{ .p_ctrl = &g_rtc0_ctrl, .p_cfg = &g_rtc0_cfg, .p_api = &g_rtc_on_rtc };
sci_b_uart_instance_ctrl_t mpy_repl_uart_ctrl;

sci_b_baud_setting_t mpy_repl_uart_baud_setting =
        {
        /* Baud rate calculated with 0.469% error. */.baudrate_bits_b.abcse = 0,
          .baudrate_bits_b.abcs = 0, .baudrate_bits_b.bgdm = 1, .baudrate_bits_b.cks = 0, .baudrate_bits_b.brr = 53, .baudrate_bits_b.mddr =
                  (uint8_t) 256,
          .baudrate_bits_b.brme = false };

/** UART extended configuration for UARTonSCI HAL driver */
const sci_b_uart_extended_cfg_t mpy_repl_uart_cfg_extend =
{ .clock = SCI_B_UART_CLOCK_INT, .rx_edge_start = SCI_B_UART_START_BIT_FALLING_EDGE, .noise_cancel =
          SCI_B_UART_NOISE_CANCELLATION_DISABLE,
  .rx_fifo_trigger = SCI_B_UART_RX_FIFO_TRIGGER_MAX, .p_baud_setting = &mpy_repl_uart_baud_setting, .flow_control =
          SCI_B_UART_FLOW_CONTROL_RTS,
#if 0xFF != 0xFF
                .flow_control_pin       = BSP_IO_PORT_FF_PIN_0xFF,
                #else
  .flow_control_pin = (bsp_io_port_pin_t) UINT16_MAX,
#endif
  .rs485_setting =
  { .enable = SCI_B_UART_RS485_DISABLE,
    .polarity = SCI_B_UART_RS485_DE_POLARITY_HIGH,
    .assertion_time = 1,
    .negation_time = 1, },
  .delay_cycles = 0, };

/** UART interface configuration */
const uart_cfg_t mpy_repl_uart_cfg =
{ .channel = 1, .data_bits = UART_DATA_BITS_8, .parity = UART_PARITY_OFF, .stop_bits = UART_STOP_BITS_1, .p_callback =
          mpy_repl_uart_callback,
  .p_context = NULL, .p_extend = &mpy_repl_uart_cfg_extend,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
  .p_transfer_tx = NULL,
#else
                .p_transfer_tx       = &RA_NOT_DEFINED,
#endif
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
  .p_transfer_rx = NULL,
#else
                .p_transfer_rx       = &RA_NOT_DEFINED,
#endif
#undef RA_NOT_DEFINED
  .rxi_ipl = (12),
  .txi_ipl = (12), .tei_ipl = (12), .eri_ipl = (12),
#if defined(VECTOR_NUMBER_SCI1_RXI)
                .rxi_irq             = VECTOR_NUMBER_SCI1_RXI,
#else
  .rxi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI1_TXI)
                .txi_irq             = VECTOR_NUMBER_SCI1_TXI,
#else
  .txi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI1_TEI)
                .tei_irq             = VECTOR_NUMBER_SCI1_TEI,
#else
  .tei_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI1_ERI)
                .eri_irq             = VECTOR_NUMBER_SCI1_ERI,
#else
  .eri_irq = FSP_INVALID_VECTOR,
#endif
        };

/* Instance structure to use this module. */
const uart_instance_t mpy_repl_uart =
{ .p_ctrl = &mpy_repl_uart_ctrl, .p_cfg = &mpy_repl_uart_cfg, .p_api = &g_uart_on_sci_b };

ospi_b_instance_ctrl_t g_ospi0_ctrl;

static ospi_b_timing_setting_t g_ospi0_timing_settings =
{ .command_to_command_interval = OSPI_B_COMMAND_INTERVAL_CLOCKS_2,
  .cs_pullup_lag = OSPI_B_COMMAND_CS_PULLUP_CLOCKS_NO_EXTENSION,
  .cs_pulldown_lead = OSPI_B_COMMAND_CS_PULLDOWN_CLOCKS_NO_EXTENSION,
  .sdr_drive_timing = OSPI_B_SDR_DRIVE_TIMING_BEFORE_CK,
  .sdr_sampling_edge = OSPI_B_CK_EDGE_RISING,
  .sdr_sampling_delay = OSPI_B_SDR_SAMPLING_DELAY_NONE,
  .ddr_sampling_extension = OSPI_B_DDR_SAMPLING_EXTENSION_NONE, };

static const spi_flash_erase_command_t g_ospi0_command_set_initial_erase_commands[] =
{
{ .command = 0x20, .size = 4096 },
  { .command = 0xD8, .size = 65536 },
  { .command = 0xC7, .size = SPI_FLASH_ERASE_SIZE_CHIP_ERASE }, };
static const ospi_b_table_t g_ospi0_command_set_initial_erase_table =
{ .p_table = (void*) g_ospi0_command_set_initial_erase_commands, .length =
          sizeof(g_ospi0_command_set_initial_erase_commands) / sizeof(g_ospi0_command_set_initial_erase_commands[0]), };

static const spi_flash_erase_command_t g_ospi0_command_set_high_speed_erase_commands[] =
{
{ .command = 0x2121, .size = 4096 },
  { .command = 0xDCDC, .size = 65536 },
  { .command = 0x6060, .size = SPI_FLASH_ERASE_SIZE_CHIP_ERASE }, };
static const ospi_b_table_t g_ospi0_command_set_high_speed_erase_table =
{ .p_table = (void*) g_ospi0_command_set_high_speed_erase_commands, .length =
          sizeof(g_ospi0_command_set_high_speed_erase_commands)
                  / sizeof(g_ospi0_command_set_high_speed_erase_commands[0]), };

static const ospi_b_xspi_command_set_t g_ospi0_command_set_table[] =
{
{ .protocol = SPI_FLASH_PROTOCOL_1S_1S_1S,
  .frame_format = OSPI_B_FRAME_FORMAT_STANDARD,
  .latency_mode = OSPI_B_LATENCY_MODE_FIXED,
  .command_bytes = OSPI_B_COMMAND_BYTES_1,
  .address_bytes = SPI_FLASH_ADDRESS_BYTES_3,
  .address_msb_mask = 0x00,
  .status_needs_address = false,
  .status_address = 0U,
  .status_address_bytes = (spi_flash_address_bytes_t) 0U,
  .p_erase_commands = &g_ospi0_command_set_initial_erase_table,
  .read_command = 0x03,
  .read_dummy_cycles = 0,
  .program_command = 0x02,
  .program_dummy_cycles = 0,
  .row_load_command = 0x0,
  .row_load_dummy_cycles = 0,
  .row_store_command = 0x0,
  .row_store_dummy_cycles = 0,
  .write_enable_command = 0x06,
  .status_command = 0x05,
  .status_dummy_cycles = 0, },
  { .protocol = SPI_FLASH_PROTOCOL_8D_8D_8D,
    .frame_format = OSPI_B_FRAME_FORMAT_XSPI_PROFILE_1,
    .latency_mode = OSPI_B_LATENCY_MODE_FIXED,
    .command_bytes = OSPI_B_COMMAND_BYTES_2,
    .address_bytes = SPI_FLASH_ADDRESS_BYTES_4,
    .address_msb_mask = 0xF0,
    .status_needs_address = true,
    .status_address = 0x00,
    .status_address_bytes = SPI_FLASH_ADDRESS_BYTES_4,
    .p_erase_commands = &g_ospi0_command_set_high_speed_erase_table,
    .read_command = 0xEEEE,
    .read_dummy_cycles = 20,
    .program_command = 0x1212,
    .program_dummy_cycles = 0,
    .row_load_command = 0x0,
    .row_load_dummy_cycles = 0,
    .row_store_command = 0x0,
    .row_store_dummy_cycles = 0,
    .write_enable_command = 0x0606,
    .status_command = 0x0505,
    .status_dummy_cycles = 3, } };

static const ospi_b_table_t g_ospi0_command_set =
{ .p_table = (void*) g_ospi0_command_set_table, .length = 2 };

#if OSPI_B_CFG_DOTF_SUPPORT_ENABLE
extern uint8_t g_ospi_dotf_iv[];
extern uint8_t g_ospi_dotf_key[];

static ospi_b_dotf_cfg_t g_ospi_dotf_cfg=
{
    .key_type       = OSPI_B_DOTF_AES_KEY_TYPE_128,
    .format         = OSPI_B_DOTF_KEY_FORMAT_PLAINTEXT,
    .p_start_addr   = (uint32_t *)0x90000000,
    .p_end_addr     = (uint32_t *)0x90001FFF,
    .p_key          = (uint32_t *)g_ospi_dotf_key,
    .p_iv           = (uint32_t *)g_ospi_dotf_iv,
};
#endif

static const ospi_b_extended_cfg_t g_ospi0_extended_cfg =
{ .ospi_b_unit = 0,
  .channel = (ospi_b_device_number_t) 1,
  .p_timing_settings = &g_ospi0_timing_settings,
  .p_xspi_command_set = &g_ospi0_command_set,
  .data_latch_delay_clocks = OSPI_B_DS_TIMING_DELAY_NONE,
  .p_autocalibration_preamble_pattern_addr = (uint8_t*) 0,
#if OSPI_B_CFG_DMAC_SUPPORT_ENABLE
    .p_lower_lvl_transfer                    = &RA_NOT_DEFINED,
#endif
#if OSPI_B_CFG_DOTF_SUPPORT_ENABLE
    .p_dotf_cfg                              = &g_ospi_dotf_cfg,
#endif
#if OSPI_B_CFG_ROW_ADDRESSING_SUPPORT_ENABLE
    .row_index_bytes                         = 0xFF,
#endif
        };
const spi_flash_cfg_t g_ospi0_cfg =
{ .spi_protocol = SPI_FLASH_PROTOCOL_1S_1S_1S, .read_mode = SPI_FLASH_READ_MODE_STANDARD, /* Unused by OSPI_B */
  .address_bytes = SPI_FLASH_ADDRESS_BYTES_3, .dummy_clocks = SPI_FLASH_DUMMY_CLOCKS_DEFAULT, /* Unused by OSPI_B */
  .page_program_address_lines = (spi_flash_data_lines_t) 0U, /* Unused by OSPI_B */
  .page_size_bytes = 64, .write_status_bit = 0, .write_enable_bit = 1, .page_program_command = 0, /* OSPI_B uses command sets. See g_ospi0_command_set. */
  .write_enable_command = 0, /* OSPI_B uses command sets. See g_ospi0_command_set. */
  .status_command = 0, /* OSPI_B uses command sets. See g_ospi0_command_set. */
  .read_command = 0, /* OSPI_B uses command sets. See g_ospi0_command_set. */
#if OSPI_B_CFG_XIP_SUPPORT_ENABLE
    .xip_enter_command           = 0,
    .xip_exit_command            = 0,
#else
  .xip_enter_command = 0U,
  .xip_exit_command = 0U,
#endif
  /* OSPI_B uses command sets, this is kept for backwards compatibility. See g_ospi0_command_set. */
  .erase_command_list_length = sizeof(g_ospi0_command_set_initial_erase_commands)
          / sizeof(g_ospi0_command_set_initial_erase_commands[0]),
  .p_erase_command_list = g_ospi0_command_set_initial_erase_commands, .p_extend = &g_ospi0_extended_cfg, };

/** This structure encompasses everything that is needed to use an instance of this interface. */
const spi_flash_instance_t g_ospi0 =
{ .p_ctrl = &g_ospi0_ctrl, .p_cfg = &g_ospi0_cfg, .p_api = &g_ospi_b_on_spi_flash, };

#if defined OSPI_B_CFG_DOTF_PROTECTED_MODE_SUPPORT_ENABLE
rsip_instance_t const * const gp_rsip_instance = &RA_NOT_DEFINED;
#endif

dmac_instance_ctrl_t g_transfer0_ctrl;
transfer_info_t g_transfer0_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_SOURCE,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_4_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 128, };
const dmac_extended_cfg_t g_transfer0_extend =
{ .offset = 1, .src_buffer_size = 1,
#if defined(VECTOR_NUMBER_DMAC0_INT)
    .irq                 = VECTOR_NUMBER_DMAC0_INT,
#else
  .irq = FSP_INVALID_VECTOR,
#endif
  .ipl = (12),
  .channel = 0, .p_callback = omv_sdmmc_dmac_callback, .p_context = &omv_sdmmc_ctrl, .activation_source =
          ELC_EVENT_SDHIMMC0_DMA_REQ, };
const transfer_cfg_t g_transfer0_cfg =
{ .p_info = &g_transfer0_info, .p_extend = &g_transfer0_extend, };
/* Instance structure to use this module. */
const transfer_instance_t g_transfer0 =
{ .p_ctrl = &g_transfer0_ctrl, .p_cfg = &g_transfer0_cfg, .p_api = &g_transfer_on_dmac };
#define RA_NOT_DEFINED (UINT32_MAX)
#if (RA_NOT_DEFINED) != (1)

/* If the transfer module is DMAC, define a DMAC transfer callback. */
#include "r_dmac.h"
extern void r_sdhi_transfer_callback(sdhi_instance_ctrl_t *p_ctrl);

void omv_sdmmc_dmac_callback(dmac_callback_args_t *p_args)
{
    r_sdhi_transfer_callback ((sdhi_instance_ctrl_t*) p_args->p_context);
}
#endif
#undef RA_NOT_DEFINED

sdhi_instance_ctrl_t omv_sdmmc_ctrl;
sdmmc_cfg_t omv_sdmmc_cfg =
{ .bus_width = SDMMC_BUS_WIDTH_4_BITS, .channel = 0, .p_callback = omv_sdhi0_callback, .p_context = NULL, .block_size =
          512,
  .card_detect = SDMMC_CARD_DETECT_CD, .write_protect = SDMMC_WRITE_PROTECT_NONE,

  .p_extend = NULL,
  .p_lower_lvl_transfer = &g_transfer0,

  .access_ipl = (12),
  .sdio_ipl = BSP_IRQ_DISABLED, .card_ipl = (12), .dma_req_ipl = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_SDHIMMC0_ACCS)
    .access_irq             = VECTOR_NUMBER_SDHIMMC0_ACCS,
#else
  .access_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SDHIMMC0_CARD)
    .card_irq               = VECTOR_NUMBER_SDHIMMC0_CARD,
#else
  .card_irq = FSP_INVALID_VECTOR,
#endif
  .sdio_irq = FSP_INVALID_VECTOR,
#if defined(VECTOR_NUMBER_SDHIMMC0_DMA_REQ)
    .dma_req_irq            = VECTOR_NUMBER_SDHIMMC0_DMA_REQ,
#else
  .dma_req_irq = FSP_INVALID_VECTOR,
#endif
        };
/* Instance structure to use this module. */
const sdmmc_instance_t omv_sdmmc =
{ .p_ctrl = &omv_sdmmc_ctrl, .p_cfg = &omv_sdmmc_cfg, .p_api = &g_sdmmc_on_sdhi };

dmac_instance_ctrl_t omv_csi_dmac_transfer_ctrl;
transfer_info_t omv_csi_dmac_transfer_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_SOURCE,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_8_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 1, };
const dmac_extended_cfg_t omv_csi_dmac_transfer_extend =
{ .offset = 1, .src_buffer_size = 1,
#if defined(VECTOR_NUMBER_DMAC0_INT)
    .irq                 = VECTOR_NUMBER_DMAC0_INT,
#else
  .irq = FSP_INVALID_VECTOR,
#endif
  .ipl = (11),
  .channel = 0, .p_callback = omv_csi_dmac_callback, .p_context = NULL, .activation_source = ELC_EVENT_NONE, };
const transfer_cfg_t omv_csi_dmac_transfer_cfg =
{ .p_info = &omv_csi_dmac_transfer_info, .p_extend = &omv_csi_dmac_transfer_extend, };
/* Instance structure to use this module. */
const transfer_instance_t omv_csi_dmac_transfer =
{ .p_ctrl = &omv_csi_dmac_transfer_ctrl, .p_cfg = &omv_csi_dmac_transfer_cfg, .p_api = &g_transfer_on_dmac };
gpt_instance_ctrl_t g_timer11_ctrl;
#if 0
const gpt_extended_pwm_cfg_t g_timer11_pwm_extend =
{
    .trough_ipl             = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT11_COUNTER_UNDERFLOW)
    .trough_irq             = VECTOR_NUMBER_GPT11_COUNTER_UNDERFLOW,
#else
    .trough_irq             = FSP_INVALID_VECTOR,
#endif
    .poeg_link              = GPT_POEG_LINK_POEG0,
    .output_disable         = (gpt_output_disable_t) ( GPT_OUTPUT_DISABLE_NONE),
    .adc_trigger            = (gpt_adc_trigger_t) ( GPT_ADC_TRIGGER_NONE),
    .dead_time_count_up     = 0,
    .dead_time_count_down   = 0,
    .adc_a_compare_match    = 0,
    .adc_b_compare_match    = 0,
    .interrupt_skip_source  = GPT_INTERRUPT_SKIP_SOURCE_NONE,
    .interrupt_skip_count   = GPT_INTERRUPT_SKIP_COUNT_0,
    .interrupt_skip_adc     = GPT_INTERRUPT_SKIP_ADC_NONE,
    .gtioca_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
    .gtiocb_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
};
#endif
const gpt_extended_cfg_t g_timer11_extend =
        { .gtioca =
        { .output_enabled = false, .stop_level = GPT_PIN_LEVEL_LOW },
          .gtiocb =
          { .output_enabled = true, .stop_level = GPT_PIN_LEVEL_LOW },
          .start_source = (gpt_source_t) (GPT_SOURCE_NONE), .stop_source = (gpt_source_t) (GPT_SOURCE_NONE), .clear_source =
                  (gpt_source_t) (GPT_SOURCE_NONE),
          .count_up_source = (gpt_source_t) (GPT_SOURCE_NONE), .count_down_source = (gpt_source_t) (GPT_SOURCE_NONE), .capture_a_source =
                  (gpt_source_t) (GPT_SOURCE_NONE),
          .capture_b_source = (gpt_source_t) (GPT_SOURCE_NONE), .capture_a_ipl = (BSP_IRQ_DISABLED), .capture_b_ipl =
                  (BSP_IRQ_DISABLED),
          .compare_match_c_ipl = (BSP_IRQ_DISABLED), .compare_match_d_ipl = (BSP_IRQ_DISABLED), .compare_match_e_ipl =
                  (BSP_IRQ_DISABLED),
          .compare_match_f_ipl = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT11_CAPTURE_COMPARE_A)
    .capture_a_irq         = VECTOR_NUMBER_GPT11_CAPTURE_COMPARE_A,
#else
          .capture_a_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT11_CAPTURE_COMPARE_B)
    .capture_b_irq         = VECTOR_NUMBER_GPT11_CAPTURE_COMPARE_B,
#else
          .capture_b_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT11_COMPARE_C)
    .compare_match_c_irq   = VECTOR_NUMBER_GPT11_COMPARE_C,
#else
          .compare_match_c_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT11_COMPARE_D)
    .compare_match_d_irq   = VECTOR_NUMBER_GPT11_COMPARE_D,
#else
          .compare_match_d_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT11_COMPARE_E)
    .compare_match_e_irq   = VECTOR_NUMBER_GPT11_COMPARE_E,
#else
          .compare_match_e_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT11_COMPARE_F)
    .compare_match_f_irq   = VECTOR_NUMBER_GPT11_COMPARE_F,
#else
          .compare_match_f_irq = FSP_INVALID_VECTOR,
#endif
          .compare_match_value =
          { (uint32_t) 0x0, /* CMP_A */
            (uint32_t) 0x0, /* CMP_B */
            (uint32_t) 0x0, /* CMP_C */
            (uint32_t) 0x0, /* CMP_D */
            (uint32_t) 0x0, /* CMP_E */
            (uint32_t) 0x0, /* CMP_F */},
          .compare_match_status = ((0U << 5U) | (0U << 4U) | (0U << 3U) | (0U << 2U) | (0U << 1U) | 0U), .capture_filter_gtioca =
                  GPT_CAPTURE_FILTER_NONE,
          .capture_filter_gtiocb = GPT_CAPTURE_FILTER_NONE,
#if 0
    .p_pwm_cfg             = &g_timer11_pwm_extend,
#else
          .p_pwm_cfg = NULL,
#endif
#if 0
    .gtior_setting.gtior_b.gtioa  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.oadflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.oahld  = 0U,
    .gtior_setting.gtior_b.oae    = (uint32_t) false,
    .gtior_setting.gtior_b.oadf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfaen  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsa  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
    .gtior_setting.gtior_b.gtiob  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.obdflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.obhld  = 0U,
    .gtior_setting.gtior_b.obe    = (uint32_t) true,
    .gtior_setting.gtior_b.obdf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfben  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsb  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
#else
          .gtior_setting.gtior = 0U,
#endif

          .gtioca_polarity = GPT_GTIOC_POLARITY_NORMAL,
          .gtiocb_polarity = GPT_GTIOC_POLARITY_NORMAL, };

const timer_cfg_t g_timer11_cfg =
{ .mode = TIMER_MODE_PWM,
/* Actual period: 4e-8 seconds. Actual duty: 50%. */.period_counts = (uint32_t) 0xa,
  .duty_cycle_counts = 0x5, .source_div = (timer_source_div_t) 0, .channel = 11, .p_callback = NULL,
  /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
  .p_context = (void*) &NULL,
#endif
  .p_extend = &g_timer11_extend,
  .cycle_end_ipl = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT11_COUNTER_OVERFLOW)
    .cycle_end_irq       = VECTOR_NUMBER_GPT11_COUNTER_OVERFLOW,
#else
  .cycle_end_irq = FSP_INVALID_VECTOR,
#endif
        };
/* Instance structure to use this module. */
const timer_instance_t g_timer11 =
{ .p_ctrl = &g_timer11_ctrl, .p_cfg = &g_timer11_cfg, .p_api = &g_timer_on_gpt };
usb_instance_ctrl_t g_basic1_ctrl;

#if !defined(g_usb_descriptor)
extern usb_descriptor_t g_usb_descriptor;
#endif
#define RA_NOT_DEFINED (1)
const usb_cfg_t g_basic1_cfg =
{ .usb_mode = USB_MODE_PERI, .usb_speed = USB_SPEED_HS, .module_number = 1, .type = USB_CLASS_PCDC,
#if defined(g_usb_descriptor)
                .p_usb_reg = g_usb_descriptor,
#else
  .p_usb_reg = &g_usb_descriptor,
#endif
  .usb_complience_cb = NULL,
#if defined(VECTOR_NUMBER_USBFS_INT)
                .irq       = VECTOR_NUMBER_USBFS_INT,
#else
  .irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_USBFS_RESUME)
                .irq_r     = VECTOR_NUMBER_USBFS_RESUME,
#else
  .irq_r = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_USBFS_FIFO_0)
                .irq_d0    = VECTOR_NUMBER_USBFS_FIFO_0,
#else
  .irq_d0 = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_USBFS_FIFO_1)
                .irq_d1    = VECTOR_NUMBER_USBFS_FIFO_1,
#else
  .irq_d1 = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_USBHS_USB_INT_RESUME)
                .hsirq     = VECTOR_NUMBER_USBHS_USB_INT_RESUME,
#else
  .hsirq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_USBHS_FIFO_0)
                .hsirq_d0  = VECTOR_NUMBER_USBHS_FIFO_0,
#else
  .hsirq_d0 = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_USBHS_FIFO_1)
                .hsirq_d1  = VECTOR_NUMBER_USBHS_FIFO_1,
#else
  .hsirq_d1 = FSP_INVALID_VECTOR,
#endif
  .ipl = (12),
  .ipl_r = (12), .ipl_d0 = (12), .ipl_d1 = (12), .hsipl = (12), .hsipl_d0 = (12), .hsipl_d1 = (12),
#if (BSP_CFG_RTOS == 0) && defined(USB_CFG_HMSC_USE)
                .p_usb_apl_callback = NULL,
#else
  .p_usb_apl_callback = NULL,
#endif
#if defined(NULL)
                .p_context = NULL,
#else
  .p_context = (void*) &NULL,
#endif
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
#else
                .p_transfer_tx = &RA_NOT_DEFINED,
#endif
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
#else
                .p_transfer_rx = &RA_NOT_DEFINED,
#endif
        };
#undef RA_NOT_DEFINED

/* Instance structure to use this module. */
const usb_instance_t g_basic1 =
{ .p_ctrl = &g_basic1_ctrl, .p_cfg = &g_basic1_cfg, .p_api = &g_usb_on_usb, };

iic_b_master_instance_ctrl_t g_i2c_master1_ctrl;
const iic_b_master_extended_cfg_t g_i2c_master1_extend =
{ .timeout_mode = IIC_B_MASTER_TIMEOUT_MODE_SHORT,
  .timeout_scl_low = IIC_B_MASTER_TIMEOUT_SCL_LOW_ENABLED,
  .smbus_operation = 0,
  /* Actual calculated bitrate: 99800. Actual calculated duty cycle: 50%. */.clock_settings.brl_value = 244,
  .clock_settings.brh_value = 243,
  .clock_settings.cks_value = 2,
  .clock_settings.sdod_value = 0,
  .clock_settings.sdodcs_value = 0,
  .iic_clock_freq = 50000000, };
const i2c_master_cfg_t g_i2c_master1_cfg =
{ .channel = 0, .rate = I2C_MASTER_RATE_STANDARD, .slave = 0x00, .addr_mode = I2C_MASTER_ADDR_MODE_7BIT,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
  .p_transfer_tx = NULL,
#else
                .p_transfer_tx       = &RA_NOT_DEFINED,
#endif
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
  .p_transfer_rx = NULL,
#else
                .p_transfer_rx       = &RA_NOT_DEFINED,
#endif
#undef RA_NOT_DEFINED
  .p_callback = g_iic_b_master0_callback,
  .p_context = NULL,
#if defined(VECTOR_NUMBER_IICB0_RXI)
    .rxi_irq             = VECTOR_NUMBER_IICB0_RXI,
#else
  .rxi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_IICB0_TXI)
    .txi_irq             = VECTOR_NUMBER_IICB0_TXI,
#else
  .txi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_IICB0_TEND)
    .tei_irq             = VECTOR_NUMBER_IICB0_TEND,
#elif defined(VECTOR_NUMBER_IICB0_TEI)
    .tei_irq             = VECTOR_NUMBER_IICB0_TEI,
#else
  .tei_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_IICB0_EEI)
    .eri_irq             = VECTOR_NUMBER_IICB0_EEI,
#elif defined(VECTOR_NUMBER_IICB0_ERI)
    .eri_irq             = VECTOR_NUMBER_IICB0_ERI,
#else
  .eri_irq = FSP_INVALID_VECTOR,
#endif
  .ipl = (12),
  .p_extend = &g_i2c_master1_extend, };
/* Instance structure to use this module. */
const i2c_master_instance_t g_i2c_master1 =
{ .p_ctrl = &g_i2c_master1_ctrl, .p_cfg = &g_i2c_master1_cfg, .p_api = &g_i2c_master_on_iic_b };
iic_master_instance_ctrl_t omv_i2c_ctrl;
const iic_master_extended_cfg_t omv_i2c_extend =
{ .timeout_mode = IIC_MASTER_TIMEOUT_MODE_SHORT,
  .timeout_scl_low = IIC_MASTER_TIMEOUT_SCL_LOW_ENABLED,
  .smbus_operation = 0,
  /* Actual calculated bitrate: 97809. Actual calculated duty cycle: 49%. */.clock_settings.brl_value = 17,
  .clock_settings.brh_value = 16,
  .clock_settings.cks_value = 4,
  .clock_settings.sddl_value = 0,
  .clock_settings.dlcs_value = 0, };
const i2c_master_cfg_t omv_i2c_cfg =
{ .channel = 0, .rate = I2C_MASTER_RATE_STANDARD, .slave = 0x00, .addr_mode = I2C_MASTER_ADDR_MODE_7BIT,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
  .p_transfer_tx = NULL,
#else
                .p_transfer_tx       = &RA_NOT_DEFINED,
#endif
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
  .p_transfer_rx = NULL,
#else
                .p_transfer_rx       = &RA_NOT_DEFINED,
#endif
#undef RA_NOT_DEFINED
  .p_callback = omv_i2c_callback,
  .p_context = NULL,
#if defined(VECTOR_NUMBER_IIC0_RXI)
    .rxi_irq             = VECTOR_NUMBER_IIC0_RXI,
#else
  .rxi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_IIC0_TXI)
    .txi_irq             = VECTOR_NUMBER_IIC0_TXI,
#else
  .txi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_IIC0_TEI)
    .tei_irq             = VECTOR_NUMBER_IIC0_TEI,
#else
  .tei_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_IIC0_ERI)
    .eri_irq             = VECTOR_NUMBER_IIC0_ERI,
#else
  .eri_irq = FSP_INVALID_VECTOR,
#endif
  .ipl = (9),
  .p_extend = &omv_i2c_extend, };
/* Instance structure to use this module. */
const i2c_master_instance_t omv_i2c =
{ .p_ctrl = &omv_i2c_ctrl, .p_cfg = &omv_i2c_cfg, .p_api = &g_i2c_master_on_iic };
void g_hal_init(void)
{
    g_common_init ();
}

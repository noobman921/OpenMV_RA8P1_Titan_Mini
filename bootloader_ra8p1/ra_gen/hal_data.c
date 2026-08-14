/* generated HAL source file - do not edit */
#include "hal_data.h"
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
          NULL,
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
{ .command = 0x20, .size = 4096 },
  { .command = 0xD8, .size = 65536 },
  { .command = 0xC7, .size = SPI_FLASH_ERASE_SIZE_CHIP_ERASE }, };
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
  .address_msb_mask = 0xF0,
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
  { .protocol = SPI_FLASH_PROTOCOL_1S_4S_4S,
    .frame_format = OSPI_B_FRAME_FORMAT_STANDARD,
    .latency_mode = OSPI_B_LATENCY_MODE_FIXED,
    .command_bytes = OSPI_B_COMMAND_BYTES_1,
    .address_bytes = SPI_FLASH_ADDRESS_BYTES_3,
    .address_msb_mask = 0xF0,
    .status_needs_address = true,
    .status_address = 0x00,
    .status_address_bytes = SPI_FLASH_ADDRESS_BYTES_3,
    .p_erase_commands = &g_ospi0_command_set_high_speed_erase_table,
    .read_command = 0xEB,
    .read_dummy_cycles = 6,
    .program_command = 0x32,
    .program_dummy_cycles = 0,
    .row_load_command = 0x0,
    .row_load_dummy_cycles = 0,
    .row_store_command = 0x0,
    .row_store_dummy_cycles = 0,
    .write_enable_command = 0x06,
    .status_command = 0x05,
    .status_dummy_cycles = 0, } };

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
#define RA_NOT_DEFINED (UINT32_MAX)
#if (RA_NOT_DEFINED != RA_NOT_DEFINED)
  void * const gp_mcuboot_flash_ctrl = &RA_NOT_DEFINED_ctrl;
  flash_cfg_t const * const gp_mcuboot_flash_cfg = &RA_NOT_DEFINED_cfg;
  flash_instance_t const * const gp_mcuboot_flash_instance = &RA_NOT_DEFINED;
#else
void *const gp_mcuboot_flash_ctrl = &g_mram0_ctrl;
flash_cfg_t const *const gp_mcuboot_flash_cfg = &g_mram0_cfg;
flash_instance_t const *const gp_mcuboot_flash_instance = &g_mram0;
#endif
#undef RA_NOT_DEFINED
void g_hal_init(void)
{
    g_common_init ();
}

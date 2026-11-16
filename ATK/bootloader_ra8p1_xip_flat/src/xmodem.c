/*
 * xmodem.c
 *
 * 通过 USB CDC 接收 XMODEM-128 格式的镜像。
 * 裸机 FSP 的 USB 状态机必须靠 R_USB_EventGet() 轮询推进（应用回调不会被调用），
 * 这里的 usb_poll() 等价于 ATK 例程 38_usb_fs_pcdc 中的 usb_uart_task()。
 */

#include "xmodem.h"

#include "hal_data.h"
#include "ospi_flash.h"

#include <string.h>

/* ---------------------------------------------------------------------------
 * XMODEM 协议常量
 * ------------------------------------------------------------------------- */
#define XM_SOH              (0x01U)
#define XM_EOT              (0x04U)
#define XM_ACK              (0x06U)
#define XM_NAK              (0x15U)
#define XM_CAN              (0x18U)

#define XM_BLOCK_SIZE       (128U)
#define XM_FRAME_SIZE       (132U)          /* SOH + 块号 + ~块号 + 128 数据 + 校验 */

#define LINE_CODING_LENGTH  (7U)

/* ---------------------------------------------------------------------------
 * USB CDC：轮询实现
 * ------------------------------------------------------------------------- */
#define USB_RX_BUF_SIZE     (64U)
#define USB_RX_RING_SIZE    (1024U)

static usb_pcdc_linecoding_t g_line_coding =
{
    .dw_dte_rate    = 115200U,
    .b_char_format  = 0U,                   /* STOP_BITS_1 */
    .b_parity_type  = 0U,                   /* PARITY_NONE */
    .b_data_bits    = 8U,                   /* DATA_BITS_8 */
};

static volatile usb_pcdc_ctrllinestate_t g_control_line_state =
{
    .bdtr = 0U,
    .brts = 0U,
};

static uint8_t           s_usb_rx_buf[USB_RX_BUF_SIZE];
static uint8_t           s_rx_ring[USB_RX_RING_SIZE];
static volatile uint32_t s_rx_head;
static volatile uint32_t s_rx_tail;
static volatile bool     s_attached;
static volatile bool     s_write_complete;

/* FSP 生成的配置里引用了这个名字，裸机下驱动不会调用它，空实现即可 */
void usb_cdc_rtos_callback(usb_callback_args_t * p_args)
{
    FSP_PARAMETER_NOT_USED(p_args);
}

static uint32_t ring_count(void)
{
    return (uint32_t) ((s_rx_head + USB_RX_RING_SIZE - s_rx_tail) % USB_RX_RING_SIZE);
}

static void usb_start_read(void)
{
    /* 刚配置完成时驱动可能还没就绪，重试几次 */
    for (uint32_t retry = 0U; retry < 20U; retry++)
    {
        if (FSP_SUCCESS == R_USB_Read(&g_basic0_ctrl, s_usb_rx_buf, (uint32_t) sizeof(s_usb_rx_buf), USB_CLASS_PCDC))
        {
            return;
        }

        R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
    }
}

/* 拉取并处理一个 USB 事件；所有等待循环里都要反复调用 */
static void usb_poll(void)
{
    usb_event_info_t info;
    usb_status_t     ev;

    memset(&info, 0, sizeof(info));

    if (FSP_SUCCESS != R_USB_EventGet(&info, &ev))
    {
        return;
    }

    switch (ev)
    {
        case USB_STATUS_CONFIGURED:
        {
            s_attached       = true;
            s_write_complete = true;
            usb_start_read();
            break;
        }

        case USB_STATUS_READ_COMPLETE:
        {
            for (uint32_t i = 0U; i < info.data_size; i++)
            {
                uint32_t next = (s_rx_head + 1U) % USB_RX_RING_SIZE;
                if (next != s_rx_tail)
                {
                    s_rx_ring[s_rx_head] = s_usb_rx_buf[i];
                    s_rx_head            = next;
                }
            }

            if (s_attached)
            {
                usb_start_read();
            }
            break;
        }

        case USB_STATUS_REQUEST:
        {
            uint16_t request = (uint16_t) (info.setup.request_type & USB_BREQUEST);

            if (USB_PCDC_SET_LINE_CODING == request)
            {
                (void) R_USB_PeriControlDataGet(&g_basic0_ctrl, (uint8_t *) &g_line_coding, LINE_CODING_LENGTH);
            }
            else if (USB_PCDC_GET_LINE_CODING == request)
            {
                (void) R_USB_PeriControlDataSet(&g_basic0_ctrl, (uint8_t *) &g_line_coding, LINE_CODING_LENGTH);
            }
            else if (USB_PCDC_SET_CONTROL_LINE_STATE == request)
            {
                g_control_line_state.bdtr = (uint8_t) ((info.setup.request_value >> 0) & 0x01U);
                g_control_line_state.brts = (uint8_t) ((info.setup.request_value >> 1) & 0x01U);
                (void) R_USB_PeriControlStatusSet(&g_basic0_ctrl, USB_SETUP_STATUS_ACK);
            }
            else
            {
                /* none */
            }
            break;
        }

        case USB_STATUS_WRITE_COMPLETE:
        {
            s_write_complete = true;
            break;
        }

        case USB_STATUS_DETACH:
        case USB_STATUS_SUSPEND:
        {
            s_attached       = false;
            s_write_complete = false;
            s_rx_head        = 0U;
            s_rx_tail        = 0U;
            break;
        }

        case USB_STATUS_RESUME:
        {
            s_attached = true;
            break;
        }

        default:
        {
            break;
        }
    }
}

static void comms_send(const uint8_t * p_src, uint32_t len)
{
    if (0U == len)
    {
        return;
    }

    while (!s_write_complete)
    {
        usb_poll();
    }

    s_write_complete = false;

    /* CONFIGURED 事件比硬件状态略早，第一次写可能失败，带重试 */
    fsp_err_t err = FSP_ERR_USB_FAILED;
    for (uint32_t retry = 0U; retry < 100U; retry++)
    {
        err = R_USB_Write(&g_basic0_ctrl, (uint8_t *) p_src, len, USB_CLASS_PCDC);
        if (FSP_SUCCESS == err)
        {
            break;
        }

        usb_poll();
        R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
    }

    if (FSP_SUCCESS != err)
    {
        s_write_complete = true;

        return;
    }

    while (!s_write_complete)
    {
        usb_poll();
    }
}

static void comms_send_str(const char * p_str)
{
    uint32_t len = 0U;

    while ('\0' != p_str[len])
    {
        len++;
    }

    comms_send((const uint8_t *) p_str, len);
}

static fsp_err_t comms_read(uint8_t * p_dest, uint32_t * p_len, uint32_t timeout_milliseconds)
{
    uint32_t need   = *p_len;
    uint32_t waited = 0U;

    for (;;)
    {
        /* 每毫秒轮询多次，跟上 USB 收包节奏 */
        for (uint32_t i = 0U; i < 20U; i++)
        {
            usb_poll();

            if (ring_count() >= need)
            {
                for (uint32_t n = 0U; n < need; n++)
                {
                    p_dest[n] = s_rx_ring[s_rx_tail];
                    s_rx_tail = (s_rx_tail + 1U) % USB_RX_RING_SIZE;
                }

                *p_len = need;

                return FSP_SUCCESS;
            }
        }

        if (waited >= timeout_milliseconds)
        {
            *p_len = 0U;

            return FSP_ERR_TIMEOUT;
        }

        waited++;
        R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
    }
}

/* ---------------------------------------------------------------------------
 * 入口
 * ------------------------------------------------------------------------- */
bool xmodem_download(uint32_t flash_address)
{
    static const char banner[] = "\r\nXMODEM ready, waiting for image...\r\n";

    uint8_t  frame[XM_FRAME_SIZE];
    uint8_t  tx_byte;
    uint8_t  expected_block = 1U;
    uint32_t address        = flash_address;
    bool     flash_ready    = false;

    s_attached       = false;
    s_write_complete = false;
    s_rx_head        = 0U;
    s_rx_tail        = 0U;

    if (FSP_SUCCESS != R_USB_Open(&g_basic0_ctrl, &g_basic0_cfg))
    {
        return false;
    }

    /* 等主机完成枚举并打开串口 */
    while (!s_attached)
    {
        usb_poll();
    }

    comms_send((const uint8_t *) banner, (uint32_t) (sizeof(banner) - 1U));

    for (;;)
    {
        /* 死等：每秒补发一次 NAK，直到收到完整一帧 */
        tx_byte = XM_NAK;
        comms_send(&tx_byte, 1U);

        uint32_t len = XM_FRAME_SIZE;
        if ((FSP_SUCCESS != comms_read(frame, &len, 1000U)) || (XM_FRAME_SIZE != len))
        {
            continue;
        }

        if (XM_EOT == frame[0])
        {
            tx_byte = XM_ACK;
            comms_send(&tx_byte, 1U);
            comms_send_str("image received\r\n");

            return true;
        }

        /* 校验：SOH + 块号 + ~块号 + 128 字节和 */
        uint8_t sum = 0U;
        for (uint32_t i = 0U; i < XM_BLOCK_SIZE; i++)
        {
            sum = (uint8_t) (sum + frame[3U + i]);
        }

        if ((XM_SOH != frame[0]) || ((uint8_t) (frame[1] + frame[2]) != 0xFFU) || (sum != frame[131]))
        {
            tx_byte = XM_NAK;
            comms_send(&tx_byte, 1U);
            continue;
        }

        if (frame[1] == expected_block)
        {
            /* 第一帧到了才初始化 OSPI，之前让 USB 安静地完成枚举 */
            if (!flash_ready)
            {
                if (FSP_SUCCESS != ospi_flash_init())
                {
                    tx_byte = XM_CAN;
                    comms_send(&tx_byte, 1U);
                    comms_send_str("ospi init failed\r\n");

                    return false;
                }

                flash_ready = true;
            }

            if (FSP_SUCCESS != ospi_flash_program(address, &frame[3], XM_BLOCK_SIZE))
            {
                tx_byte = XM_CAN;
                comms_send(&tx_byte, 1U);
                comms_send_str("flash write failed\r\n");

                return false;
            }

            address += XM_BLOCK_SIZE;
            expected_block++;
        }

        tx_byte = XM_ACK;
        comms_send(&tx_byte, 1U);
    }
}

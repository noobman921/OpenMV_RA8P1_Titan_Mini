/*
 * ospi_flash.c
 *
 * 外部 OSPI NOR flash（OSPI0 CS0）的初始化与写入。
 * 初始化流程与 ATK 裸机例程 10_norflash 的 norflash_init() 一致。
 */

#include "ospi_flash.h"

#include <string.h>

#define OSPI_FLASH_SECTOR_SIZE  (4096U)

#define OSPI_FLASH_CMD_WREN     (0x06U)
#define OSPI_FLASH_CMD_RSTEN    (0x66U)
#define OSPI_FLASH_CMD_RESET    (0x99U)

static bool s_opened;

static void ospi_flash_send_cmd(uint16_t command)
{
    spi_flash_direct_transfer_t transfer =
    {
        .command        = command,
        .command_length = 1U,
        .address        = 0U,
        .address_length = 0U,
        .data           = 0U,
        .data_length    = 0U,
        .dummy_cycles   = 0U,
    };

    (void) R_OSPI_B_DirectTransfer(&g_ospi0_ctrl, &transfer, SPI_FLASH_DIRECT_TRANSFER_DIR_WRITE);
}

static fsp_err_t ospi_flash_wait_idle(void)
{
    spi_flash_status_t status  = { .write_in_progress = true };
    uint32_t           timeout = 1000000U;

    while (timeout-- > 0U)
    {
        fsp_err_t err = R_OSPI_B_StatusGet(&g_ospi0_ctrl, &status);
        if (FSP_SUCCESS != err)
        {
            return err;
        }

        if (false == status.write_in_progress)
        {
            return FSP_SUCCESS;
        }
    }

    return FSP_ERR_TIMEOUT;
}

fsp_err_t ospi_flash_init(void)
{
    /* 已打开过就先关掉，保证每次都从干净状态初始化 */
    if (s_opened)
    {
        (void) R_OSPI_B_Close(&g_ospi0_ctrl);
        s_opened = false;
    }

    fsp_err_t err = R_OSPI_B_Open(&g_ospi0_ctrl, &g_ospi0_cfg);
    if (FSP_SUCCESS != err)
    {
        return err;
    }
    s_opened = true;

    /* CS0 采样模式 */
    R_XSPI0->LIOCFGCS_b[0].SDRSMPMD = 0x1;

    ospi_flash_send_cmd(OSPI_FLASH_CMD_RSTEN);
    ospi_flash_send_cmd(OSPI_FLASH_CMD_RESET);
    R_BSP_SoftwareDelay(30, BSP_DELAY_UNITS_MICROSECONDS);
    ospi_flash_send_cmd(OSPI_FLASH_CMD_WREN);

    /* 手动命令发完后 memory-mapped 读不再生效，重开一次把它恢复 */
    (void) R_OSPI_B_Close(&g_ospi0_ctrl);
    err = R_OSPI_B_Open(&g_ospi0_ctrl, &g_ospi0_cfg);
    if (FSP_SUCCESS != err)
    {
        s_opened = false;

        return err;
    }

    s_opened = true;
    R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);

    return FSP_SUCCESS;
}

fsp_err_t ospi_flash_program(uint32_t address, const uint8_t * p_data, uint32_t size)
{
    static uint32_t erased_sector = 0xFFFFFFFFUL;

    uint32_t offset = 0U;

    while (offset < size)
    {
        uint32_t sector = address & ~(OSPI_FLASH_SECTOR_SIZE - 1U);

        /* 进入新扇区前先擦除 */
        if (sector != erased_sector)
        {
            fsp_err_t err = R_OSPI_B_Erase(&g_ospi0_ctrl, (uint8_t *) sector, OSPI_FLASH_SECTOR_SIZE);
            if (FSP_SUCCESS != err)
            {
                return err;
            }

            err = ospi_flash_wait_idle();
            if (FSP_SUCCESS != err)
            {
                return err;
            }

            erased_sector = sector;
        }

        uint32_t chunk = OSPI_FLASH_SECTOR_SIZE - (address - sector);
        if (chunk > (size - offset))
        {
            chunk = size - offset;
        }

        /* 按 64 字节分块写（匹配 OSPI 的 64 字节组合写），
         * 源数据先拷进 8 字节对齐的缓冲——R_OSPI_B_Write 内部按 uint64_t 访问。 */
        uint32_t done = 0U;
        while (done < chunk)
        {
            uint8_t  buf[64] BSP_ALIGN_VARIABLE(8);
            uint32_t n = chunk - done;
            if (n > sizeof(buf))
            {
                n = sizeof(buf);
            }

            memcpy(buf, p_data + offset + done, n);

            fsp_err_t err = R_OSPI_B_Write(&g_ospi0_ctrl, buf, (uint8_t *) (address + done), n);
            if (FSP_SUCCESS != err)
            {
                return err;
            }

            err = ospi_flash_wait_idle();
            if (FSP_SUCCESS != err)
            {
                return err;
            }

            done += n;
        }

        address += chunk;
        offset  += chunk;
    }

    return FSP_SUCCESS;
}

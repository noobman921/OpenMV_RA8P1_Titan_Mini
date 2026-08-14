#include <downloader_thread.h>
#include "header.h"
#include "comms.h"
#include "stdio.h"
#include "menu.h"
#include "qspi_operations.h"

void stack_overflow_error_occurred (void);


void display_image_slot_info(void)
{
    uint8_t str[100];

    struct image_version {
        uint8_t iv_major;
        uint8_t iv_minor;
        uint16_t iv_revision;
        uint32_t iv_build_num;
    };

    struct image_header {
        uint32_t ih_magic;
        uint32_t ih_load_addr;
        uint16_t ih_hdr_size;            /* Size of image header (bytes). */
        uint16_t ih_protect_tlv_size;    /* Size of protected TLV area (bytes). */
        uint32_t ih_img_size;            /* Does not include header. */
        uint32_t ih_flags;               /* IMAGE_F_[...]. */
        struct image_version ih_ver;
        uint32_t _pad1;
    };

    struct image_header * p_img_header;

    /* Primary Image Slot */
    p_img_header = (struct image_header *)PRIMARY_IMAGE_START_ADDRESS;
    snprintf((char *)str, sizeof(str), "*************************\r\n");
    comms_send(str, strlen((char *)str));
    snprintf((char *)str, sizeof(str), "* Primary Image Slot *\r\n");
    comms_send(str, strlen((char *)str));
    snprintf((char *)str, sizeof(str), "*************************\r\n");
    comms_send(str, strlen((char *)str));
    snprintf((char *)str, sizeof(str), "Image version: \t\t%02d.%02d (Rev: %d, Build: %ld)\r\n", p_img_header->ih_ver.iv_major,
                                                                                             p_img_header->ih_ver.iv_minor,
                                                                                             p_img_header->ih_ver.iv_revision,
                                                                                             p_img_header->ih_ver.iv_build_num);
    comms_send(str, strlen((char *)str));
    snprintf((char *)str, sizeof(str), "Primary image start address: \t\t0x%08X\r\n", (unsigned int)PRIMARY_IMAGE_START_ADDRESS);
    comms_send(str, strlen((char *)str));
    snprintf((char *)str, sizeof(str), "Header size: \t\t0x%04X (%d bytes)\r\n", p_img_header->ih_hdr_size, p_img_header->ih_hdr_size);
    comms_send(str, strlen((char *)str));
    snprintf((char *)str, sizeof(str), "Protected TLV size: \t0x%04X (%d bytes)\r\n", p_img_header->ih_protect_tlv_size, p_img_header->ih_protect_tlv_size);
    comms_send(str, strlen((char *)str));
    snprintf((char *)str, sizeof(str), "Image size: \t\t0x%08X (%ld bytes)\r\n", (unsigned int)p_img_header->ih_img_size, p_img_header->ih_img_size);
    comms_send(str, strlen((char *)str));

    /* Secondary S Image Slot */
    p_img_header = (struct image_header *)SECONDARY_IMAGE_START_ADDRESS;
    snprintf((char *)str, sizeof(str), "\r\n**************************\r\n");
    comms_send(str, strlen((char *)str));
    snprintf((char *)str, sizeof(str), "* Secondary Image Slot *\r\n");
    comms_send(str, strlen((char *)str));
    snprintf((char *)str, sizeof(str), "**************************\r\n");
    comms_send(str, strlen((char *)str));
    snprintf((char *)str, sizeof(str), "Image version: \t\t%02d.%02d (Rev: %d, Build: %ld)\r\n", p_img_header->ih_ver.iv_major,
                                                                                             p_img_header->ih_ver.iv_minor,
                                                                                             p_img_header->ih_ver.iv_revision,
                                                                                             p_img_header->ih_ver.iv_build_num);
    comms_send(str, strlen((char *)str));
    snprintf((char *)str, sizeof(str), "Secondary image start address: \t\t0x%08X\r\n", (unsigned int)SECONDARY_IMAGE_START_ADDRESS);
    comms_send(str, strlen((char *)str));
    snprintf((char *)str, sizeof(str), "Header size: \t\t0x%04X (%d bytes)\r\n", p_img_header->ih_hdr_size, p_img_header->ih_hdr_size);
    comms_send(str, strlen((char *)str));
    snprintf((char *)str, sizeof(str), "Protected TLV size: \t0x%04X (%d bytes)\r\n", p_img_header->ih_protect_tlv_size, p_img_header->ih_protect_tlv_size);
    comms_send(str, strlen((char *)str));
    snprintf((char *)str, sizeof(str), "Image size: \t\t0x%08X (%ld bytes)\r\n", (unsigned int)p_img_header->ih_img_size, p_img_header->ih_img_size);
    comms_send(str, strlen((char *)str));


}

usb_callback_t g_usb_cb;

#include <string.h>   // 提供 memcmp

/**
 * @brief QSPI Flash 读写测试（通过内存映射直接读取）
 * @param base_addr  QSPI 的映射基址（例如 0x90000000）
 * @param offset     Flash 内部的偏移地址（相对于基址，必须 4KB 对齐用于擦除）
 * @param size       测试数据大小（建议 <= 256）
 * @retval 0 成功, -1 失败
 */
int qspi_flash_mem_test(uint32_t base_addr, uint32_t offset, uint32_t size)
{
    fsp_err_t err;
    uint8_t write_buf[256];
    uint8_t read_buf[256];
    uint32_t i;

    // 目标物理地址（在 QSPI 映射空间中的绝对地址）
    uint8_t * target_addr = (uint8_t *)(base_addr + offset);
    // 擦除地址需向下对齐到 4KB 边界（因为擦除最小单位是扇区）
    uint32_t sector_addr = ((uint32_t)target_addr) & ~0xFFF;

    // 1. 填充测试数据（0x00~0xFF 循环，便于观察）
    for (i = 0; i < size; i++) {
        write_buf[i] = (uint8_t)(i & 0xFF);
    }

    // 2. 擦除扇区（注意：此时如果 CPU 正在执行该区域的代码，必定崩溃！）
    err = R_OSPI_B_Erase(&g_ospi0_ctrl, (uint8_t *)sector_addr, 4096);
    if (err != FSP_SUCCESS) {
        return -1;
    }

    // 3. 写入测试数据
    err = R_OSPI_B_Write(&g_ospi0_ctrl, write_buf, target_addr, size);
    if (err != FSP_SUCCESS) {
        return -1;
    }

    // 4. 关键步骤：直接通过指针读取（不需要 R_OSPI_B_Read）
    //    方法 A：逐字节复制（推荐）
    for (i = 0; i < size; i++) {
        read_buf[i] = target_addr[i];
    }
    //    方法 B：如果支持，也可以用 memcpy（底层会转为字节读取）
    //    memcpy(read_buf, target_addr, size);

    // 5. 比较数据
    if (memcmp(write_buf, read_buf, size) != 0) {
        return -1;  // 数据不一致
    }

    return 0;  // 测试通过
}


/* New Thread entry function */
/* pvParameters contains TaskHandle_t */
void downloader_thread_entry(void *pvParameters)
{
   FSP_PARAMETER_NOT_USED (pvParameters);
   fsp_err_t err;
/* Open the comms driver */
   err = comms_open();
   if (FSP_SUCCESS != err)
   {
       /* Stop as comms open failure */
       while(1)
       {
           ;
       }
   }
//   err =   R_FLASH_HP_Open(&g_flash0_ctrl, &g_flash0_cfg);
//   init_qspi();
   err = R_OSPI_B_Open(&g_ospi0_ctrl, &g_ospi0_cfg);
//
//   int ret = qspi_flash_mem_test(0x90000000, 0x100000, 256);
//   if (ret == 0) {
//       // 成功
//       vTaskDelay(1);
//   } else {
//       // 失败
//       vTaskDelay(1);
//   }

    /* TODO: add your own code here */
    while (1)
    {
        menu();
        vTaskDelay (1);
    }
}

#ifndef OSPI_FLASH_H
#define OSPI_FLASH_H

#include <stdint.h>
#include "hal_data.h"

/* 初始化外部 OSPI NOR flash（CS0）：
 * Open -> 采样模式 -> 0x66/0x99 复位 -> 写使能 -> 重开以恢复 memory-mapped 读。
 * 可重复调用。 */
fsp_err_t ospi_flash_init(void);

/* 把 size 字节写入 OSPI 的 address 处；需要时自动擦除所跨的 4KB 扇区。
 * 源数据任意对齐，内部会拷到 8 字节对齐缓冲再写。 */
fsp_err_t ospi_flash_program(uint32_t address, const uint8_t * p_data, uint32_t size);

#endif /* OSPI_FLASH_H */

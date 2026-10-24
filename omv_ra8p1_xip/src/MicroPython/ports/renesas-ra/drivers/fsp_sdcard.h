/*
 * fsp_sdcard.h
 *
 *  Created on: 2026年9月8日
 *      Author: qq292
 */

#ifndef MICROPYTHON_PORTS_RENESAS_RA_DRIVERS_FSP_SDCARD_H_
#define MICROPYTHON_PORTS_RENESAS_RA_DRIVERS_FSP_SDCARD_H_

#include "mpy_board_cfg.h"

fsp_err_t SDCard_Init(void);
int32_t SDCard_ReadBlocks(uint32_t start_block, uint8_t *buf, uint32_t block_count);
int32_t SDCard_WriteBlocks(uint32_t start_block, const uint8_t *buf, uint32_t block_count);

/* 串行化 FatFS(MPY Thread) 与 MSC(msc_io worker) 对同一 R_SDHI 的访问。
 * 必须在调度器启动后、任何一次读写之前调用（SDCard_Init 已经调了）。 */
void SDCard_LockInit(void);

uint32_t SDCard_GetSectorCount(void);
uint32_t SDCard_GetSectorSize(void);

#endif /* MICROPYTHON_PORTS_RENESAS_RA_DRIVERS_FSP_SDCARD_H_ */

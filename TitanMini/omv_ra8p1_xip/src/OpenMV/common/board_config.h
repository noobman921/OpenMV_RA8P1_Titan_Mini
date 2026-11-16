/*
 * SPDX-License-Identifier: MIT
 *
 * 上游命名适配层：OpenMV 1.7.3 用 "board_config.h" 作为板级配置头，
 * 本工程历史上叫 "omv_boardconfig.h"（在 src/OpenMV/boards/RA8/）。
 *
 * 目的：让从上游整份拷进来的文件（common/umalloc.c 等）**保持零改动**，
 * 同时又能拿到我们的板级配置。这样后续再对齐上游时不会产生新的命名 diff。
 *
 * 与 Titan 的做法一致：Titan_Mini_Openmv/platform/include/board_config.h
 * 就是他们那份板级配置（等价于我们的 omv_boardconfig.h）。
 */
#ifndef __BOARD_CONFIG_H__
#define __BOARD_CONFIG_H__

#include "omv_boardconfig.h"

#endif // __BOARD_CONFIG_H__

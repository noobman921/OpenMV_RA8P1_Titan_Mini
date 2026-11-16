/*
 * fsp_sdcard.c
 *
 *  Created on: 2026年9月8日
 *      Author: qq292
 */

#include <stdio.h>
#include "fsp_sdcard.h"
#include "FreeRTOS.h"
#include "semphr.h"

#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
    #include "core_cm85.h"
#endif

static uint32_t sdcard_sector_size  = 512;
static uint32_t sdcard_sector_count = 0;

volatile bool sdcard_transfer_complete = false;
volatile bool sdcard_transfer_failed = false;
volatile bool sdcard_inserted = false;
volatile uint32_t sdcard_erase_complete = 0;

/* 串行化对 R_SDHI 的访问。
 *
 * 同一个控制器有两个访问者、跑在两个不同优先级的任务里：
 *   - FatFS（MicroPython VFS）在 MPY Thread（优先级 3）
 *   - MSC（U 盘）在 msc_io worker（优先级 2）
 * 而 R_SDHI 只有一份寄存器状态，`sdcard_transfer_complete` 也只有一个。
 * 没有这把锁时，高优先级一方会在对方传输中途插进来，后完成的传输把前一个的
 * 等待者“叫醒”，两者都会拿着错误的数据继续 —— U 盘会直接读坏/写坏。
 *
 * 对应 Titan 的做法：titan_storage.c 的 storage_usb_transfer() 用 dfs_lock()
 * 把 MSC 的介质访问与本地 FAT 串行化。
 *
 * 用二值信号量而不是互斥量：本工程 configUSE_MUTEXES = 0。
 * worker 优先级低于 VM，不会造成优先级反转（VM 等待时 worker 一定能跑完）。 */
static SemaphoreHandle_t sdcard_lock;

void SDCard_LockInit(void) {
    if (sdcard_lock == NULL) {
        sdcard_lock = xSemaphoreCreateBinary();
        if (sdcard_lock != NULL) {
            xSemaphoreGive(sdcard_lock);
        }
    }
}

static inline void sdcard_lock_take(void) {
    if (sdcard_lock != NULL) {
        xSemaphoreTake(sdcard_lock, portMAX_DELAY);
    }
}

static inline void sdcard_lock_give(void) {
    if (sdcard_lock != NULL) {
        xSemaphoreGive(sdcard_lock);
    }
}

uint32_t SDCard_GetSectorCount(void) {
    return sdcard_sector_count;
}

uint32_t SDCard_GetSectorSize(void) {
    return sdcard_sector_size;
}

fsp_err_t SDCard_Init(void){
    /* 幂等：本函数会在两处被调用 ——
     *   ① omvmain.c 里 tusb_init() 之前（让 MSC 在枚举时就报告正确容量）
     *   ② sdcard_flash_init_fs() 里（挂 FatFS 之前）
     * 第二处必须是个 no-op，否则会重复 R_SDHI_Open（FSP 会返回 ALREADY_OPEN）
     * 并再等 1 秒。 */
    static bool sdcard_opened = false;
    static bool sdcard_ready = false;
    fsp_err_t err = FSP_SUCCESS;

    if (sdcard_ready) {
        return FSP_SUCCESS;
    }

    if (!sdcard_opened) {
        SDCard_LockInit();
        err = R_SDHI_Open(&RA_SDCARD_CTRL, &RA_SDCARD_CFG);
        if (FSP_SUCCESS != err)
        {
          return err;
        }
        sdcard_opened = true;
        R_BSP_SoftwareDelay(1000U, BSP_DELAY_UNITS_MILLISECONDS);
    }
    // 检查sdcard是否插入
    sdmmc_status_t status;
    err = R_SDHI_StatusGet(&RA_SDCARD_CTRL, &status);
    if (FSP_SUCCESS != err)
    {
      return err;
    }
    if (status.card_inserted == false)
    {
      return FSP_ERR_NOT_OPEN;
    }
    R_BSP_SoftwareDelay(1U, BSP_DELAY_UNITS_MILLISECONDS);

    // 初始化sdcard
    sdmmc_device_t sdmmc_dev = {0};
    err = R_SDHI_MediaInit(&RA_SDCARD_CTRL, &sdmmc_dev);
    if (FSP_SUCCESS != err)
    {
      return err;
    }
    sdcard_sector_size = sdmmc_dev.sector_size_bytes;
    sdcard_sector_count = sdmmc_dev.sector_count;
    sdcard_inserted = true;
    sdcard_ready = true;
    return err;
}

int32_t SDCard_ReadBlocks(uint32_t start_block, uint8_t *buf, uint32_t block_count){
    fsp_err_t err = FSP_SUCCESS;
    sdcard_lock_take();

    /* 发起传输前先清零，避免残留的完成标志让下面的等待被跳过 */
    sdcard_transfer_complete = false;
    sdcard_transfer_failed = false;
    err = R_SDHI_Read(&RA_SDCARD_CTRL, buf, start_block, block_count);
    if (FSP_SUCCESS != err)
    {
      sdcard_lock_give();
      return -1;
    }
    /* 出错由 omv_sdhi0_callback 置 failed 立即放行；两者都没来则靠有限次自旋兜底，
     * 绝不无限等待。 */
    int32_t timeout = INT32_MAX;
    while(!sdcard_transfer_complete && !sdcard_transfer_failed){
        if (--timeout <= 0) {
            sdcard_lock_give();
            return -1;
        }
    }
    if (sdcard_transfer_failed) {
        sdcard_lock_give();
        return -1;
    }
    sdcard_transfer_complete = false;
    /* 等待 DMA 写回内存完成后再做 cache 操作 */
    __DSB();
#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
    SCB_InvalidateDCache_by_Addr(buf, block_count * sdcard_sector_size);
#endif
    __DSB();
    __ISB();
    sdcard_lock_give();
    return 0;
}

int32_t SDCard_WriteBlocks(uint32_t start_block, const uint8_t *buf, uint32_t block_count){
    fsp_err_t err = FSP_SUCCESS;
    sdcard_lock_take();
#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
    SCB_CleanDCache_by_Addr((uint32_t *)buf, block_count * sdcard_sector_size);
#endif
    /* 发起传输前先清零，避免残留的完成标志让下面的等待被跳过 */
    sdcard_transfer_complete = false;
    sdcard_transfer_failed = false;
    err = R_SDHI_Write(&RA_SDCARD_CTRL, buf, start_block, block_count);
    if (FSP_SUCCESS != err)
    {
      sdcard_lock_give();
      return -1;
    }
    int32_t timeout = INT32_MAX;
    while(!sdcard_transfer_complete && !sdcard_transfer_failed){
        if (--timeout <= 0) {
            sdcard_lock_give();
            return -1;
        }
    }
    if (sdcard_transfer_failed) {
        sdcard_lock_give();
        return -1;
    }
    sdcard_transfer_complete = false;
    sdcard_lock_give();
    return 0;
}

void omv_sdhi0_callback(sdmmc_callback_args_t *p_args){
    if (SDMMC_EVENT_TRANSFER_COMPLETE == p_args->event)  //读取或写入完成
    {
        sdcard_transfer_complete = true;
    }
    if (SDMMC_EVENT_CARD_INSERTED == p_args->event)  //卡插入中断
    {
        sdcard_inserted = true;
    }
    if (SDMMC_EVENT_CARD_REMOVED == p_args->event)   //卡拔出中断
    {
        sdcard_inserted = false;
    }
    if (SDMMC_EVENT_ERASE_COMPLETE == p_args->event)  //擦除完成
    {
        sdcard_erase_complete = 1;
    }
    if (SDMMC_EVENT_ERASE_BUSY == p_args->event)  //擦除超时
    {
        sdcard_erase_complete = 2;
    }
    if (SDMMC_EVENT_TRANSFER_ERROR == p_args->event)
    {
        sdcard_erase_complete = 3;
        /* 放行 SDCard_Read/WriteBlocks 里的等待循环。
         * 此前错误事件从不设置完成标志，等待者（tud_task 所在的 svc 线程）
         * 会被无限期冻住，USB 无响应 → Windows 复位端口 → CDC 会话被杀。 */
        sdcard_transfer_failed = true;
    }
}

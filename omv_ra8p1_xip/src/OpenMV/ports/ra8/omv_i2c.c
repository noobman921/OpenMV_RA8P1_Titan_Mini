#include <stdio.h>
#include <stdbool.h>
#include "hal_data.h"

#include "py/mphal.h"

#include "omv_boardconfig.h"
#include "omv_i2c.h"

uint8_t omv_i2c_rx_status = 0;
uint8_t omv_i2c_tx_status = 0;
#define I2C_SCAN_TIMEOUT_CNT   10
#define I2C_WRITE_TIMEOUT_CNT   1000

static i2c_master_cfg_t omv_i2c_runtime_cfg;

int omv_i2c_init(omv_i2c_t *i2c, uint32_t bus_id, uint32_t speed) {
    i2c->id = bus_id;
    i2c->initialized = false;
    i2c->speed = speed;
    i2c->inst = OMV_I2C_HANDLE;
    omv_i2c_runtime_cfg = OMV_I2C_CFG;
    switch(speed){
        case OMV_I2C_SPEED_STANDARD:
            omv_i2c_runtime_cfg.rate = I2C_MASTER_RATE_STANDARD;
            break;
        case OMV_I2C_SPEED_FULL:
            omv_i2c_runtime_cfg.rate = I2C_MASTER_RATE_FAST;
            break;
        case OMV_I2C_SPEED_FAST:
            omv_i2c_runtime_cfg.rate = I2C_MASTER_RATE_FASTPLUS;
            break;
        case OMV_I2C_SPEED_MAX:
            omv_i2c_runtime_cfg.rate = I2C_MASTER_RATE_HIGHSPEED;
            break;
    }
    fsp_err_t err = R_IIC_MASTER_Open(&OMV_I2C_HANDLE, &omv_i2c_runtime_cfg);
    R_BSP_SoftwareDelay (100, BSP_DELAY_UNITS_MILLISECONDS);
    i2c->initialized = true;
    return 0;
}

int omv_i2c_deinit(omv_i2c_t *i2c) {
    if (i2c->initialized) {
    	R_IIC_MASTER_Close(&OMV_I2C_HANDLE);
    }
    i2c->initialized = false;
    return 0;
}

//扫描辅助函数
bool ra_i2c_scan(omv_i2c_t *i2c, uint8_t addr_7bit)
{
    uint8_t rx_buf = 0;
    uint32_t cnt = 0;
    fsp_err_t err;

    if (i2c == NULL)
    {
        return false;
    }
    omv_i2c_rx_status = 0;

    // 设置I2C从地址
    err = R_IIC_MASTER_SlaveAddressSet(&OMV_I2C_HANDLE, addr_7bit, I2C_MASTER_ADDR_MODE_7BIT);
    if (err != FSP_SUCCESS)
    {
        return false;
    }

    // 发起读探测
    err = R_IIC_MASTER_Read(&OMV_I2C_HANDLE, &rx_buf, 1U, 0U);
    if (err != FSP_SUCCESS)
    {
        return false;
    }
    // 循环 + 时间超时
    uint32_t tick_start = mp_hal_ticks_ms();
    while (1)
    {
        if (omv_i2c_rx_status == 1)
        {
            omv_i2c_rx_status = 0;
            return true;
        }

        // 超时
        if (mp_hal_ticks_ms() - tick_start >= I2C_SCAN_TIMEOUT_CNT)
        {
            R_IIC_MASTER_Abort(&OMV_I2C_HANDLE);
            omv_i2c_rx_status = 0;
            return false;
        }

        __NOP();
    }

    return false;
}

//扫描I2C总线上的设备地址，返回找到的设备数量，并将地址列表存储在提供的缓冲区中（如果有的话）。
int omv_i2c_scan(omv_i2c_t *i2c, uint8_t *list, uint8_t size) {
    int idx = 0;
    for (uint8_t addr = 0x15; addr <= 0x77; addr++)
        {
            if (ra_i2c_scan(i2c, addr))
            {
                if (list == NULL || size == 0)
                {
                    return (addr << 1);
                }
                else if (idx < size)
                {
                    list[idx++] = (addr << 1);
                }
                else
                {
                    break;
                }
            }
        }
    return idx;
}

int omv_i2c_enable(omv_i2c_t *i2c, bool enable) {
    return 0;
}

//向I2C总线上的所有设备发送一个通用调用命令
int omv_i2c_gencall(omv_i2c_t *i2c, uint8_t cmd) {
    R_IIC_MASTER_SlaveAddressSet(&OMV_I2C_HANDLE, 0x00, I2C_MASTER_ADDR_MODE_7BIT);
    fsp_err_t error = R_IIC_MASTER_Write(&OMV_I2C_HANDLE, &cmd, 1, 0);
    if(error!=FSP_SUCCESS){
    	return -1;
    }
    return 0;
}

//向特定的I2C设备读取数据
int omv_i2c_read(omv_i2c_t *i2c, uint8_t slv_addr, uint8_t *buf, uint32_t len, uint32_t flags) {

	// 8位地址转7位
	fsp_err_t error;
	bool nostop = false;
	uint8_t addr_7bit = slv_addr >> 1U;
	error = R_IIC_MASTER_SlaveAddressSet(&OMV_I2C_HANDLE, addr_7bit, I2C_MASTER_ADDR_MODE_7BIT);
	if (error != FSP_SUCCESS)
	{
		return -1;
	}

	if (flags & OMV_I2C_XFER_NO_STOP)
	{
		nostop = true;
	}

	error = R_IIC_MASTER_Read(&OMV_I2C_HANDLE, buf, len, nostop);
	if (error != FSP_SUCCESS)
	{
		return -1;
	}
	uint32_t tick_start = mp_hal_ticks_ms();
	while (1)
	{
		if (omv_i2c_rx_status == 1)
		{
			omv_i2c_rx_status = 0;
			return 0;
		}

		// 超时 1 秒
		if (mp_hal_ticks_ms() - tick_start >= I2C_SCAN_TIMEOUT_CNT)
		{
			R_IIC_MASTER_Abort(&OMV_I2C_HANDLE);
			omv_i2c_rx_status = 0;
			return -1;
		}

		__NOP();
	}
	return 0;
}

//向特定的I2C设备写入数据
int omv_i2c_write(omv_i2c_t *i2c, uint8_t slv_addr, uint8_t *buf, uint32_t len, uint32_t flags) {
	// 8位地址转7位
	fsp_err_t error;
	bool nostop = false;
	uint8_t addr_7bit = slv_addr >> 1U;
	error = R_IIC_MASTER_SlaveAddressSet(&OMV_I2C_HANDLE, addr_7bit, I2C_MASTER_ADDR_MODE_7BIT);
	if (error != FSP_SUCCESS)
	{
	    printf("[I2C] Slave error \n");
		return -1;
	}

	if (flags & OMV_I2C_XFER_NO_STOP)
	{
		nostop = true;
	}

	error = R_IIC_MASTER_Write(&OMV_I2C_HANDLE, buf, len, nostop);
	if (error != FSP_SUCCESS)
	{
	    printf("[I2C] Write error \n");
		return -1;
	}
	uint32_t tick_start = mp_hal_ticks_ms();
	while (1)
	{
		if (omv_i2c_tx_status == 1)
		{
			omv_i2c_tx_status = 0;
			return 0;
		}

		// 超时 1 秒
		if (mp_hal_ticks_ms() - tick_start >= I2C_WRITE_TIMEOUT_CNT)
		{
			R_IIC_MASTER_Abort(&OMV_I2C_HANDLE);
			omv_i2c_tx_status = 0;
			printf("[I2C] timeout \n");
			return -1;
		}

		__NOP();
	}
	return 0;

}

//重置I2C总线，尝试恢复被锁定的设备(未实现)
int omv_i2c_pulse_scl(omv_i2c_t *i2c) {
//	R_IIC_B_MASTER_Close(&OMV_I2C_HANDLE);
//
//	R_IIC_B_MASTER_Open(&OMV_I2C_HANDLE, &OMV_I2C_CFG);


    // 未实现
    return 0;
}

void omv_i2c_callback(i2c_master_callback_args_t *p_args){
	switch(p_args->event){
		case I2C_MASTER_EVENT_RX_COMPLETE:
			omv_i2c_rx_status = 1;
			break;
		case I2C_MASTER_EVENT_TX_COMPLETE:
			omv_i2c_tx_status = 1;
			break;
		default:
			break;
	}
}

/*
 * user_printf.c
 *
 *  Created on: 2026年7月10日
 *      Author: qq292
 */

#include <unistd.h>
#include "hal_data.h"


int _write(int file, const char *ptr, size_t len) {
    (void)file;
    for (size_t i = 0; i < len; i++) {

      R_SCI_B_UART_Write(&g_uart0_ctrl, (uint8_t*)&ptr[i], 1);
      FSP_HARDWARE_REGISTER_WAIT((&g_uart0_ctrl)->p_reg->CSR_b.TEND, 1U);
    }
    return len;
}

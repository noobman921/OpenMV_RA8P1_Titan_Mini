#include "blinky_thread.h"
/* Blinky Thread entry function */
/* pvParameters contains TaskHandle_t */
void blinky_thread_entry(void *pvParameters)
{
    FSP_PARAMETER_NOT_USED (pvParameters);

    /* TODO: add your own code here */
    while (1)
    {
        R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_08, BSP_IO_LEVEL_LOW);
        vTaskDelay (1000);
        R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_08, BSP_IO_LEVEL_HIGH);
        vTaskDelay (1000);
    }
}

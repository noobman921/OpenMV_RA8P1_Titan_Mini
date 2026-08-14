#include "mpy_thread.h"

extern int omv_main();

/* MPY Thread entry function */
/* pvParameters contains TaskHandle_t */
void mpy_thread_entry(void *pvParameters)
{
    FSP_PARAMETER_NOT_USED (pvParameters);

    /* TODO: add your own code here */
    omv_main();
    while (1)
    {
        vTaskDelay (1);
    }
}

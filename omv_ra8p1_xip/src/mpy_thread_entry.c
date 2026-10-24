#include <mpy_thread.h>
/* New Thread entry function */
/* pvParameters contains TaskHandle_t */

extern int omv_main();

void mpy_thread_entry(void *pvParameters)
{
    FSP_PARAMETER_NOT_USED (pvParameters);

    /* TODO: add your own code here */
    omv_main();
}

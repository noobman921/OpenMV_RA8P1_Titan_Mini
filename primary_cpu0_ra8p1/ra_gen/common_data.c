/* generated common source file - do not edit */
#include "common_data.h"
mram_instance_ctrl_t g_mram0_ctrl;
const flash_cfg_t g_mram0_cfg =
{ .data_flash_bgo = false, .p_callback = NULL, .p_context = NULL,
#if defined(VECTOR_NUMBER_MRAM_MRCRD)
    .err_irq        = VECTOR_NUMBER_MRAM_MRCRD,
#else
  .err_irq = FSP_INVALID_VECTOR,
#endif
  .err_ipl = (BSP_IRQ_DISABLED),
  .irq = FSP_INVALID_VECTOR, .ipl = BSP_IRQ_DISABLED, };
/* Instance structure to use this module. */
const flash_instance_t g_mram0 =
{ .p_ctrl = &g_mram0_ctrl, .p_cfg = &g_mram0_cfg, .p_api = &g_flash_on_mram };
ioport_instance_ctrl_t g_ioport_ctrl;
const ioport_instance_t g_ioport =
{ .p_api = &g_ioport_on_ioport, .p_ctrl = &g_ioport_ctrl, .p_cfg = &g_bsp_pin_cfg, };
SemaphoreHandle_t g_usb_write_complete_binary_semaphore;
#if 1
StaticSemaphore_t g_usb_write_complete_binary_semaphore_memory;
#endif
void rtos_startup_err_callback(void *p_instance, void *p_data);
QueueHandle_t g_usb_read_queue;
#if 1
StaticQueue_t g_usb_read_queue_memory;
uint8_t g_usb_read_queue_queue_memory[4 * 20];
#endif
void rtos_startup_err_callback(void *p_instance, void *p_data);
void g_common_init(void)
{
    g_usb_write_complete_binary_semaphore =
#if 1
            xSemaphoreCreateBinaryStatic (&g_usb_write_complete_binary_semaphore_memory);
#else
                xSemaphoreCreateBinary();
                #endif
    if (NULL == g_usb_write_complete_binary_semaphore)
    {
        rtos_startup_err_callback (g_usb_write_complete_binary_semaphore, 0);
    }
    g_usb_read_queue =
#if 1
            xQueueCreateStatic (
#else
                xQueueCreate(
                #endif
                                20,
                                4
#if 1
                                ,
                                &g_usb_read_queue_queue_memory[0], &g_usb_read_queue_memory
#endif
                                );
    if (NULL == g_usb_read_queue)
    {
        rtos_startup_err_callback (g_usb_read_queue, 0);
    }
}

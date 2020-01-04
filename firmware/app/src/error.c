#include "error.h"
#include "sys_gpio.h"
#include "stm32f1xx_hal.h"

void SYS_ThrowError(SYS_ErrorType_t err_type, uint32_t line_number, const char* file_name, const char* err_msg)
{
    switch (err_type)
    {
        case SYSERR_WARNING:
        case SYSERR_ERROR:
            //* NON-FATAL ERROR HANDLING
            break;
        case SYSERR_FATAL_ERROR:
            // Light up the error LED to signal a fatal error.
            if (hw_gpio_init_status == SYS_INIT_STATUS_INITIALISED)
            {
                HAL_GPIO_WritePin(GPIO_ERROR_LED_PORT, GPIO_ERROR_LED_PIN, GPIO_PIN_SET);
            }
            // Terminate the execution by locking the processor in an infinite loop.
    	    while (1) {}
            break;
    }
}
#include "error.h"
#include "sys_gpio.h"
#include "sys_usart.h"
#include "stm32f1xx_hal.h"
#include <stdio.h>
#include <stdlib.h>

static void _SYS_Error_NotifyDebug(uint32_t line_number, const char* file_name, const char* err_msg, const char* type);

void SYS_ThrowError(SYS_ErrorType_t err_type, uint32_t line_number, const char* file_name, const char* err_msg)
{
    switch (err_type)
    {
        case SYSERR_WARNING:
            // Print the warning message to the debug interface.
            _SYS_Error_NotifyDebug(line_number, file_name, err_msg, "WARNING");
            break;
        case SYSERR_ERROR:
            // Print the error message to the debug interface.
            _SYS_Error_NotifyDebug(line_number, file_name, err_msg, "ERROR");
            break;
        case SYSERR_FATAL_ERROR:
            // Print the error message to the debug interface.
            _SYS_Error_NotifyDebug(line_number, file_name, err_msg, "FATAL ERROR");
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

static void _SYS_Error_NotifyDebug(uint32_t line_number, const char* file_name, const char* err_msg, const char* type)
{
    char *cbuf;
    uint16_t len;

    if (hw_uart1_init_status == SYS_INIT_STATUS_INITIALISED)
    {
        cbuf = (char *) malloc(196 * sizeof(char));
        if (cbuf != NULL)
        {
            len = sprintf(cbuf, "[BKF][%s] Thrown at %s:%ld: -> %s\n", type, file_name, line_number, err_msg);
            HAL_UART_Transmit(hw_uart1_handle, (uint8_t *) cbuf, len, 10);
            free(cbuf);
        }
    }
}

// Linked with Error_Handler of HAL library.
void Error_Handler(void)
{
    SYS_ThrowError(SYSERR_FATAL_ERROR, __LINE__, __FILE__, "Error Handler invoked.");
}
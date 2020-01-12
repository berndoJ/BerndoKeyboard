#include "debug.h"
#include "sys_def.h"
#include "sys_usart.h"
#include "stm32f1xx_hal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void SYS_PrintDebugMessage(char *msg)
{
    char *buf;
    uint16_t len;

    if (hw_uart1_init_status == SYS_INIT_STATUS_INITIALISED)
    {
        len = strlen(msg) + 16;
        buf = (char *) malloc(len * sizeof(char));
        if (buf == NULL)
            return;
        len = sprintf(buf, "[Debug][Info] %s\n", msg);
        HAL_UART_Transmit(hw_uart1_handle, (uint8_t *) buf, len, 10);
        free(buf);
    }
}

void SYS_DumpMemToDebugInterface(void *mem_ptr, uint16_t mem_len)
{
    char *cbuf;
    uint8_t mbuf, li = 0;
    uint16_t i, clen;

    if (hw_uart1_init_status != SYS_INIT_STATUS_INITIALISED)
        return;

    cbuf = (char *) malloc(64 * sizeof(char));
    if (cbuf == NULL)
        return;
    clen = sprintf(cbuf, "[Debug] Memory dump @ %p [%d Bytes]:\n[Debug] ", mem_ptr, mem_len);
    HAL_UART_Transmit(hw_uart1_handle, (uint8_t *) cbuf, clen, 10);

    for (i = 0; i < mem_len; i++)
    {
        li++;

        mbuf = *((uint8_t *) (mem_ptr + i));
        clen = sprintf(cbuf, "%02X", mbuf);
        HAL_UART_Transmit(hw_uart1_handle, (uint8_t *) cbuf, clen, 10);

        if (i < mem_len - 1)
        {
            if (li == 32)
            {
                li = 0;
                HAL_UART_Transmit(hw_uart1_handle, (uint8_t *) "\n[Debug] ", 9, 10);
            }
            else
            {
                HAL_UART_Transmit(hw_uart1_handle, (uint8_t *) " ", 1, 10);
            }
        }
        else
        {
            HAL_UART_Transmit(hw_uart1_handle, (uint8_t *) "\n", 1, 10);
        }
        
    }

    HAL_UART_Transmit(hw_uart1_handle, (uint8_t *) "[Debug] Memory dump complete.\n", 30, 10);

    free(cbuf);
}
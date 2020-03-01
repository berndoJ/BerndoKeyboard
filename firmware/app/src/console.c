#include "console.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

static UART_HandleTypeDef *_console_usart = NULL;

void CONSOLE_Init(UART_HandleTypeDef *console_usart_handle)
{
    if (console_usart_handle == NULL)
        return;
    
    _console_usart = console_usart_handle;
}

void CONSOLE_Print(const char *format, ...)
{
    va_list args;
    uint32_t flen;
    char *fmsg;

    // Check for valid handle.
    if (_console_usart == NULL)
        return;

    // Allocate the buffer for fmsg.
    fmsg = (char *) malloc(strlen(format) + 256);

    if (fmsg == NULL)
        return;

    // sprintf the message with format to fmsg.
    va_start(args, format);
    flen = (uint32_t) vsprintf(fmsg, format, args);
    va_end(args);

    // Print to U(S)ART.
    HAL_UART_Transmit(_console_usart, (uint8_t *) fmsg, flen, 10);

    // De-allocate memory for message.
    free(fmsg);
}

void CONSOLE_vPrint(const char *format, va_list args)
{
    uint32_t flen;
    char *fmsg;

    // Check for valid handle.
    if (_console_usart == NULL)
        return;

    // Allocate the buffer for fmsg.
    fmsg = (char *) malloc(strlen(format) + 256);

    if (fmsg == NULL)
        return;

    // sprintf the message with format to fmsg.
    flen = (uint32_t) vsprintf(fmsg, format, args);

    // Print to U(S)ART.
    HAL_UART_Transmit(_console_usart, (uint8_t *) fmsg, flen, 10);

    // De-allocate memory for message.
    free(fmsg);
}

void CONSOLE_PrintLn(const char *format, ...)
{
    va_list args;

    va_start(args, format);
    CONSOLE_vPrint(format, args);
    va_end(args);

    CONSOLE_Print("\n");
}

void CONSOLE_ClearScreen(void)
{
    CONSOLE_Print("\033[2J\033[H");
}
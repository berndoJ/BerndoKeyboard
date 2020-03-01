/*******************************************************************************
* \file console.h
* \date 01.03.2020
********************************************************************************
* \author Johannes Berndorfer (berndoJ)
* \copyright Copyright (c) 2020 by Johannes Berndorfer (berndoJ)
********************************************************************************
* \brief Module implementing the debug console of this firmware.
*******************************************************************************/

#if !defined(__CONSOLE_H)
#define __CONSOLE_H

#include "stm32f1xx_hal.h"

void CONSOLE_Init(UART_HandleTypeDef *console_usart_handle);

void CONSOLE_Print(const char *format, ...);
void CONSOLE_PrintLn(const char *format, ...);
void CONSOLE_ClearScreen(void);

#endif // __CONSOLE_H
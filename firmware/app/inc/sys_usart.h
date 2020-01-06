/*******************************************************************************
* \file sys_usart.h
* \date 05.01.2020
********************************************************************************
* \author Johannes Berndorfer (berndoJ)
* \copyright Copyright (c) 2020 by Johannes Berndorfer (berndoJ)
********************************************************************************
* \brief Interface module for all U(S)ART ports on the host processor.
*******************************************************************************/

#if !defined(__SYS_USART_H)
#define __SYS_USART_H

#include "sys_def.h"
#include "stm32f1xx_hal.h"

#define SYS_UART1_BAUD_RATE     115200U

extern UART_HandleTypeDef *hw_uart1_handle;
extern SYS_InitStatus_t hw_uart1_init_status;

void SYS_USART_Init(void);

#endif // __SYS_USART_H
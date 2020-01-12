/*******************************************************************************
* \file sys_timer.h
* \date 06.01.2020
********************************************************************************
* \author Johannes Berndorfer (berndoJ)
* \copyright Copyright (c) 2020 by Johannes Berndorfer (berndoJ)
********************************************************************************
* \brief Initialisation module for all timers of the processor.
*******************************************************************************/

#if !defined(__SYS_TIMER_H)
#define __SYS_TIMER_H

#include "sys_def.h"
#include "stm32f1xx_hal.h"

extern TIM_HandleTypeDef *hw_timer2_handle;
extern DMA_HandleTypeDef *hw_timer2_ch3_dma_handle;
extern SYS_InitStatus_t hw_timer2_init_status;

/**
 * \brief Initialises all timers.
 */ 
void SYS_Timer_Init(void);

#endif // __SYS_TIMER_H
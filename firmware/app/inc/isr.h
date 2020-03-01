/*******************************************************************************
* \file isr.h
* \date 03.01.2020
********************************************************************************
* \author Johannes Berndorfer (berndoJ)
* \copyright Copyright (c) 2020 by Johannes Berndorfer (berndoJ)
********************************************************************************
* \brief Provides routines to the microcontroller which are called when
*        the corresponding interrupts are triggered.
*******************************************************************************/

#if !defined(__ISR_H)
#define __ISR_H

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void USB_LP_CAN1_RX0_IRQHandler(void);


#endif // __ISR_H
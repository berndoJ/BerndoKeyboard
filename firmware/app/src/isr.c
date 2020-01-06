#include "isr.h"
#include "sys_timer.h"
#include "stm32f1xx_hal.h"

void NMI_Handler(void)
{

}

void HardFault_Handler(void)
{
    while (1) {}
}

void MemManage_Handler(void)
{
    while (1) {}
}

void BusFault_Handler(void)
{
    while (1) {}
}

void UsageFault_Handler(void)
{
    while (1) {}
}

void SVC_Handler(void)
{

}

void DebugMon_Handler(void)
{

}

void PendSV_Handler(void)
{

}

void SysTick_Handler(void)
{
    HAL_IncTick();
}

void DMA1_Channel3_IRQHandler(void)
{
    HAL_DMA_IRQHandler(hw_timer3_dma_handle);
}
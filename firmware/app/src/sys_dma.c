#include "sys_dma.h"
#include "stm32f1xx_hal.h"

void SYS_DMA_Init(void)
{
    #if (__SYS_ENABLE_LIGHTING_DRIVER == 1)

    __HAL_RCC_DMA1_CLK_ENABLE();

    HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

    #endif // (__SYS_ENABLE_LIGHTING_DRIVER == 1)
}
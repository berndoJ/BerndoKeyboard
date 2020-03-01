#include "isr.h"
#include "sys_timer.h"
#include "sys_gpio.h"
#include "sys_ws2812.h"
#include "neopixel32.h"
#include "stm32f1xx_hal.h"

extern PCD_HandleTypeDef hpcd_USB_FS;

void NMI_Handler(void)
{

}

void HardFault_Handler(void)
{
    if (hw_gpio_init_status == SYS_INIT_STATUS_INITIALISED)
    {
        HAL_GPIO_WritePin(GPIO_ERROR_LED_PORT, GPIO_ERROR_LED_PIN, 1);
        HAL_GPIO_WritePin(GPIO_USER_LED_0_PORT, GPIO_USER_LED_0_PIN, 0);
        HAL_GPIO_WritePin(GPIO_USER_LED_1_PORT, GPIO_USER_LED_1_PIN, 0);
        HAL_GPIO_WritePin(GPIO_USER_LED_2_PORT, GPIO_USER_LED_2_PIN, 0);
    }
    while (1) {}
}

void MemManage_Handler(void)
{
    if (hw_gpio_init_status == SYS_INIT_STATUS_INITIALISED)
    {
        HAL_GPIO_WritePin(GPIO_ERROR_LED_PORT, GPIO_ERROR_LED_PIN, 1);
        HAL_GPIO_WritePin(GPIO_USER_LED_0_PORT, GPIO_USER_LED_0_PIN, 1);
        HAL_GPIO_WritePin(GPIO_USER_LED_1_PORT, GPIO_USER_LED_1_PIN, 0);
        HAL_GPIO_WritePin(GPIO_USER_LED_2_PORT, GPIO_USER_LED_2_PIN, 0);
    }
    while (1) {}
}

void BusFault_Handler(void)
{
    if (hw_gpio_init_status == SYS_INIT_STATUS_INITIALISED)
    {
        HAL_GPIO_WritePin(GPIO_ERROR_LED_PORT, GPIO_ERROR_LED_PIN, 1);
        HAL_GPIO_WritePin(GPIO_USER_LED_0_PORT, GPIO_USER_LED_0_PIN, 0);
        HAL_GPIO_WritePin(GPIO_USER_LED_1_PORT, GPIO_USER_LED_1_PIN, 1);
        HAL_GPIO_WritePin(GPIO_USER_LED_2_PORT, GPIO_USER_LED_2_PIN, 0);
    }
    while (1) {}
}

void UsageFault_Handler(void)
{
    if (hw_gpio_init_status == SYS_INIT_STATUS_INITIALISED)
    {
        HAL_GPIO_WritePin(GPIO_ERROR_LED_PORT, GPIO_ERROR_LED_PIN, 1);
        HAL_GPIO_WritePin(GPIO_USER_LED_0_PORT, GPIO_USER_LED_0_PIN, 1);
        HAL_GPIO_WritePin(GPIO_USER_LED_1_PORT, GPIO_USER_LED_1_PIN, 1);
        HAL_GPIO_WritePin(GPIO_USER_LED_2_PORT, GPIO_USER_LED_2_PIN, 0);
    }
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

void DMA1_Channel1_IRQHandler(void)
{
    HAL_DMA_IRQHandler(hw_timer2_ch3_dma_handle);
    NP32_DMAComplete_Callback(hw_ws2812_handle);
}

void USB_LP_CAN1_RX0_IRQHandler(void)
{
    HAL_PCD_IRQHandler(&hpcd_USB_FS);
}


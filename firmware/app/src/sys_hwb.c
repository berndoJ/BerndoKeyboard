#include "sys_hwb.h"
#include "sys_gpio.h"
#include "stm32f1xx_hal.h"

uint8_t sys_hwbyte;

void SYS_HWB_Read(void)
{
    uint8_t buf = 0U;
    buf |= ((uint8_t) HAL_GPIO_ReadPin(GPIO_HWBIT_0_PORT, GPIO_HWBIT_0_PIN));
    buf |= ((uint8_t) HAL_GPIO_ReadPin(GPIO_HWBIT_1_PORT, GPIO_HWBIT_1_PIN) << 1);
    buf |= ((uint8_t) HAL_GPIO_ReadPin(GPIO_HWBIT_2_PORT, GPIO_HWBIT_2_PIN) << 2);
    buf |= ((uint8_t) HAL_GPIO_ReadPin(GPIO_HWBIT_3_PORT, GPIO_HWBIT_3_PIN) << 3);
    buf |= ((uint8_t) HAL_GPIO_ReadPin(GPIO_HWBIT_4_PORT, GPIO_HWBIT_4_PIN) << 4);
    sys_hwbyte = buf;
}
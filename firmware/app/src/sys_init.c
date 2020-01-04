#include "sys_init.h"
#include "sys_gpio.h"
#include "stm32f1xx_hal.h"

void SYS_Init(void)
{
    // Flash the three user LEDs and the error LED on startup for the user to confirm that these indicators are operational.
    HAL_GPIO_WritePin(GPIO_USER_LED_0_PORT, GPIO_USER_LED_0_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIO_USER_LED_1_PORT, GPIO_USER_LED_1_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIO_USER_LED_2_PORT, GPIO_USER_LED_2_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIO_ERROR_LED_PORT, GPIO_ERROR_LED_PIN, GPIO_PIN_SET);
    HAL_Delay(200);
    HAL_GPIO_WritePin(GPIO_USER_LED_0_PORT, GPIO_USER_LED_0_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIO_USER_LED_1_PORT, GPIO_USER_LED_1_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIO_USER_LED_2_PORT, GPIO_USER_LED_2_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIO_ERROR_LED_PORT, GPIO_ERROR_LED_PIN, GPIO_PIN_RESET);
}
#include "main.h"
#include "sys_def.h"
#include "sys_init.h"
#include "sys_clock.h"
#include "sys_gpio.h"
#include "sys_i2c.h"
#include "stm32f1xx_hal.h"

int main(void)
{
    HAL_Init();

    SYS_Clock_Init();
    SYS_GPIO_Init();

    SYS_Init();

    SYS_I2C_Init();

    while (1)
    {
        HAL_Delay(100);
        HAL_GPIO_WritePin(GPIO_USER_LED_0_PORT, GPIO_USER_LED_0_PIN, GPIO_PIN_SET);
        HAL_Delay(100);
        HAL_GPIO_WritePin(GPIO_USER_LED_0_PORT, GPIO_USER_LED_0_PIN, GPIO_PIN_RESET);
    }

    return 0;
}
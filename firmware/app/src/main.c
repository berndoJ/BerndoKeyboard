#include "main.h"
#include "sys_def.h"
#include "sys_dma.h"
#include "sys_init.h"
#include "sys_clock.h"
#include "sys_gpio.h"
#include "sys_i2c.h"
#include "sys_usart.h"
#include "sys_pca9555.h"
#include "sys_timer.h"
#include "sys_hwb.h"
#include "sys_ws2812.h"
#include "pca9555.h"
#include "neopixel32.h"
#include "stm32f1xx_hal.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(void)
{
    // Initialise the HAL (libstm32f1hal) library.
    HAL_Init();

    // Initialise clock, GPIO and DMA.
    SYS_Clock_Init();
    SYS_GPIO_Init();
    SYS_DMA_Init();

    SYS_Init();

    // Initialise interfaces and timers.
    SYS_I2C_Init();
    SYS_USART_Init();
    SYS_Timer_Init();

    SYS_PostPortInit();

    // Initialise libraries.
    SYS_PCA9555_Init();
    SYS_WS2812_Init();

    SYS_PostLibInit();

    // Send init complete message.
    HAL_UART_Transmit(hw_uart1_handle, (uint8_t *)"[BKF] System init complete. Starting firmware.\n", 47, 10);

    // Read HW-Bits.
    SYS_HWB_Read();
    char *cbuf = malloc(40 * sizeof(char));
    uint8_t len = (uint8_t) sprintf(cbuf, "[BKF] Read hwbits: [%d][%d][%d][%d][%d]\n", SYS_HWBIT(0), SYS_HWBIT(1), SYS_HWBIT(2), SYS_HWBIT(3), SYS_HWBIT(4));
    HAL_UART_Transmit(hw_uart1_handle, (uint8_t *)cbuf, len, 10);
    free(cbuf);

    //! TEST START

    NP32_SetAllLEDs_RGB(hw_ws2812_handle, (NP32_RGB_t){ 0xff, 0x00, 0x00 });
    NP32_Update(hw_ws2812_handle);


    //! TEST END

    PCA9555_ConfigPortPinMode(hw_portex0_handle, 0, 0x00); // Config PORT0 as OUTPUT.
    PCA9555_ConfigPortPinMode(hw_portex0_handle, 1, 0x00); // Config PORT1 as OUTPUT.
    PCA9555_ConfigPortPinMode(hw_portex1_handle, 0, 0x00); // Config PORT0 as OUTPUT.
    PCA9555_ConfigPortPinMode(hw_portex1_handle, 1, 0x00); // Config PORT1 as OUTPUT.

    uint8_t x = 0;

    while (1)
    {
        HAL_Delay(100);

        x++;
        
        uint8_t y = x % 8;
        uint8_t z = x / 8;
        
        if (z >= 1 && y >= 7)
            x = 0;

        PCA9555_WritePort(hw_portex0_handle, !z, 0x00);
        PCA9555_WritePort(hw_portex0_handle, z, 0x01 << y);
    }

    return 0;
}

void HAL_MspInit(void)
{
    __HAL_RCC_AFIO_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();
}
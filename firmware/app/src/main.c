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
#include "sys_usb.h"
#include "lib_infinikeys.h"
#include "error.h"
#include "console.h"
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

    if (hw_uart1_init_status != SYS_INIT_STATUS_INITIALISED)
        SYS_ThrowError(SYSERR_FATAL_ERROR, __LINE__, __FILE__, "Failed to initialise HW-UART-1.");

    CONSOLE_Init(hw_uart1_handle);
    SYS_Timer_Init();

    SYS_PostPortInit();

    CONSOLE_ClearScreen();
    CONSOLE_PrintLn(WELCOMEMSG_LINE_0);
    CONSOLE_PrintLn(WELCOMEMSG_LINE_1);
    CONSOLE_PrintLn(WELCOMEMSG_LINE_2);
    CONSOLE_PrintLn(WELCOMEMSG_LINE_3);
    CONSOLE_PrintLn(WELCOMEMSG_LINE_4);
    CONSOLE_PrintLn(WELCOMEMSG_LINE_5);
    CONSOLE_PrintLn("BerndoKeyboard Firmware v%i.%i.%i.", MODULE_APP_VER_MAJ, MODULE_APP_VER_MIN, MODULE_APP_VER_REV);
    CONSOLE_Print("\n");

    CONSOLE_PrintLn("[BKF] Initialising USB...");
    
    #if (__SYS_ENABLE_USB_DRIVER == 1)
    SYS_USB_Init();
    #endif

    CONSOLE_PrintLn("[BKF] Initialising libraries...");

    // Initialise libraries.
    #if (__SYS_ENABLE_PORTEX_DRIVER == 1)
    SYS_PCA9555_Init();
    #endif
    #if (__SYS_ENABLE_LIGHTING_DRIVER == 1)
    SYS_WS2812_Init();
    #endif
    #if (__SYS_ENABLE_INFINIKEYS == 1)
    LIB_Infinikeys_Init();
    #endif
    
    SYS_PostLibInit();

    // Send init complete message.
    CONSOLE_PrintLn("[BKF] System init complete. Starting firmware.");

    // Read HW-Bits.
    SYS_HWB_Read();
    CONSOLE_PrintLn("[BKF] Read HW-Bits: [%d][%d][%d][%d][%d]", SYS_HWBIT(0), SYS_HWBIT(1), SYS_HWBIT(2), SYS_HWBIT(3), SYS_HWBIT(4));

    // Black out all LEDs.
    if (hw_ws2812_init_status == SYS_INIT_STATUS_INITIALISED)
    {
        NP32_ClearAllLEDs(hw_ws2812_handle);
        NP32_Update(hw_ws2812_handle);
    }

    if (hw_portex0_init_status == SYS_INIT_STATUS_INITIALISED && hw_portex1_init_status == SYS_INIT_STATUS_INITIALISED)
    {
        PCA9555_ConfigPortPinMode(hw_portex0_handle, 0, 0x00); // Config PORT0 as OUTPUT.
        PCA9555_ConfigPortPinMode(hw_portex0_handle, 1, 0x00); // Config PORT1 as OUTPUT.
        PCA9555_ConfigPortPinMode(hw_portex1_handle, 0, 0x00); // Config PORT0 as OUTPUT.
        PCA9555_ConfigPortPinMode(hw_portex1_handle, 1, 0x00); // Config PORT1 as OUTPUT.
    }

    while (1)
    {
        // Invoke InfiniKeys tick function.
        #if (__SYS_ENABLE_INFINIKEYS == 1)
        IK_Tick();
        IK_KBM_SendHIDReport();
        #endif
    }

    return 0;
}

void HAL_MspInit(void)
{
    __HAL_RCC_AFIO_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();
}
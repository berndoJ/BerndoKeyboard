#include "sys_i2c.h"
#include "error.h"
#include <stdlib.h>

I2C_HandleTypeDef *hw_i2c1_handle;
SYS_InitStatus_t hw_i2c1_init_status = SYS_INIT_STATUS_NOT_INITIALISED;

static I2C_HandleTypeDef _hw_i2c1_handle;

static void _SYS_I2C1_MspInitCallback(I2C_HandleTypeDef *handle);
static void _SYS_I2C1_MspDeInitCallback(I2C_HandleTypeDef *handle);

void SYS_I2C_Init(void)
{
    hw_i2c1_handle = &_hw_i2c1_handle;

    hw_i2c1_handle->Instance = I2C1;
    hw_i2c1_handle->Init.ClockSpeed = SYS_I2C1_CLOCK_FREQ;
    hw_i2c1_handle->Init.DutyCycle = I2C_DUTYCYCLE_2;
    hw_i2c1_handle->Init.OwnAddress1 = SYS_I2C1_HOST_ADDRESS_1;
    hw_i2c1_handle->Init.OwnAddress2 = SYS_I2C1_HOST_ADDRESS_2;
    hw_i2c1_handle->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hw_i2c1_handle->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hw_i2c1_handle->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hw_i2c1_handle->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

    hw_i2c1_handle->MspInitCallback = _SYS_I2C1_MspInitCallback;
    hw_i2c1_handle->MspDeInitCallback = _SYS_I2C1_MspDeInitCallback;

    if (HAL_I2C_Init(hw_i2c1_handle) != HAL_OK)
        SYS_ThrowError(SYSERR_FATAL_ERROR, __LINE__, __FILE__, "I2C1 INIT FAIL");
    else
        hw_i2c1_init_status = SYS_INIT_STATUS_INITIALISED;
}

static void _SYS_I2C1_MspInitCallback(I2C_HandleTypeDef *handle)
{
    GPIO_InitTypeDef gpio_init = {0};

    // Enable the GPIO clock.
    __HAL_RCC_GPIOB_CLK_ENABLE();

    // Configure the GPIO pins.
    gpio_init.Pin = GPIO_PIN_6 | GPIO_PIN_7;
    gpio_init.Mode = GPIO_MODE_AF_OD;
    gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &gpio_init);

    // Enable the I2C clock.
    __HAL_RCC_I2C1_CLK_ENABLE();
}

static void _SYS_I2C1_MspDeInitCallback(I2C_HandleTypeDef *handle)
{
    // Disable the clocks.
    __HAL_RCC_I2C1_CLK_DISABLE();

    // DeInit the GPIO pins.
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6 | GPIO_PIN_7);
}
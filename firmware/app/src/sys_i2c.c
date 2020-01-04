#include "sys_i2c.h"
#include "error.h"
#include <stdlib.h>

I2C_HandleTypeDef* hw_i2c1_handle;
SYS_InitStatus_t hw_i2c1_init_status = SYS_INIT_STATUS_NOT_INITIALISED;

void SYS_I2C_Init(void)
{
    hw_i2c1_handle = (I2C_HandleTypeDef*) malloc(sizeof(I2C_HandleTypeDef));

    hw_i2c1_handle->Instance = I2C1;
    hw_i2c1_handle->Init.ClockSpeed = SYS_I2C1_CLOCK_FREQ;
    hw_i2c1_handle->Init.DutyCycle = I2C_DUTYCYCLE_2;
    hw_i2c1_handle->Init.OwnAddress1 = SYS_I2C1_HOST_ADDRESS_1;
    hw_i2c1_handle->Init.OwnAddress2 = SYS_I2C1_HOST_ADDRESS_2;
    hw_i2c1_handle->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hw_i2c1_handle->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hw_i2c1_handle->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hw_i2c1_handle->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

    if (HAL_I2C_Init(hw_i2c1_handle) != HAL_OK)
        SYS_ThrowError(SYSERR_FATAL_ERROR, __LINE__, __FILE__, "I2C1 INIT FAIL");
    else
        hw_i2c1_init_status = SYS_INIT_STATUS_INITIALISED;
}
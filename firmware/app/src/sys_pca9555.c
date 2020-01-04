#include "sys_pca9555.h"
#include "sys_i2c.h"
#include "stm32f1xx_hal.h"
#include <stdlib.h>

PCA9555_Handle_t* hw_portex0_handle;
PCA9555_Handle_t* hw_portex1_handle;

static PCA9555_LibConfig_t* _pca9555_lib_cfg;

static void _SYS_PCA9555_I2C_Write_Func(uint8_t port_id, uint8_t device_address, uint8_t len, uint8_t *buf);
static uint8_t* _SYS_PCA9555_I2C_Read_Func(uint8_t port_id, uint8_t device_address, uint8_t len);

void SYS_PCA9555_Init(void)
{
    _pca9555_lib_cfg = (PCA9555_LibConfig_t*) malloc(sizeof(PCA9555_LibConfig_t));

    _pca9555_lib_cfg->I2C_Write = _SYS_PCA9555_I2C_Write_Func;
    _pca9555_lib_cfg->I2C_Read = _SYS_PCA9555_I2C_Read_Func;

    PCA9555_LibInit(_pca9555_lib_cfg);

    hw_portex0_handle = (PCA9555_Handle_t*) malloc(sizeof(PCA9555_Handle_t));
    hw_portex1_handle = (PCA9555_Handle_t*) malloc(sizeof(PCA9555_Handle_t));

    hw_portex0_handle->I2C_Port_ID = 0x01;
    hw_portex0_handle->I2C_Device_Address = 0x20;
    hw_portex0_handle->Port0_PinState_Buffer = 0xff;
    hw_portex0_handle->Port1_PinState_Buffer = 0xff;
    hw_portex0_handle->Port0_InputInversion_Buffer = 0x00;
    hw_portex0_handle->Port1_InputInversion_Buffer = 0x00;
    hw_portex0_handle->Port0_PinMode_Buffer = 0xff;
    hw_portex0_handle->Port1_PinMode_Buffer = 0xff;

    hw_portex1_handle->I2C_Port_ID = 0x01;
    hw_portex1_handle->I2C_Device_Address = 0x21;
    hw_portex1_handle->Port0_PinState_Buffer = 0xff;
    hw_portex1_handle->Port1_PinState_Buffer = 0xff;
    hw_portex1_handle->Port0_InputInversion_Buffer = 0x00;
    hw_portex1_handle->Port1_InputInversion_Buffer = 0x00;
    hw_portex1_handle->Port0_PinMode_Buffer = 0xff;
    hw_portex1_handle->Port1_PinMode_Buffer = 0xff;
}

static void _SYS_PCA9555_I2C_Write_Func(uint8_t port_id, uint8_t device_address, uint8_t len, uint8_t *buf)
{
    if (hw_i2c1_init_status != SYS_INIT_STATUS_INITIALISED)
        return;

    HAL_I2C_Master_Transmit(hw_i2c1_handle, device_address, buf, len, SYS_PCA9555_I2C_TIMEOUT_MS);
}

static uint8_t* _SYS_PCA9555_I2C_Read_Func(uint8_t port_id, uint8_t device_address, uint8_t len)
{
    if (hw_i2c1_init_status != SYS_INIT_STATUS_INITIALISED)
        return NULL;
    
    uint8_t* buf = (uint8_t*) malloc(sizeof(uint8_t) * len);
    HAL_I2C_Master_Receive(hw_i2c1_handle, device_address, buf, len, SYS_PCA9555_I2C_TIMEOUT_MS);
    return buf;
}
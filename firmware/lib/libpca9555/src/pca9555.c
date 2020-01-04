#include "pca9555.h"

static PCA9555_LibConfig_t _lib_cfg = {0};

static void _PCA9555_WriteRegister(PCA9555_Handle_t *handle, uint8_t reg, uint8_t data);
static uint8_t _PCA9555_ReadRegister(PCA9555_Handle_t *handle, uint8_t reg);
static void _PCA9555_UpdateCfgRegisterFromBuf(PCA9555_Handle_t *handle, uint8_t port);
static void _PCA9555_UpdateInvRegisterFromBuf(PCA9555_Handle_t *handle, uint8_t port);
static void _PCA9555_UpdateStateRegisterFromBuf(PCA9555_Handle_t *handle, uint8_t port);

void PCA9555_LibInit(PCA9555_LibConfig_t *cfg)
{
    _lib_cfg = *cfg;
}

void PCA9555_WritePin(PCA9555_Handle_t *handle, uint8_t port, uint8_t pin, uint8_t state)
{
    // Check for the parameters to be in range.
    if (!(port <= 1 && pin <= 7))
        return;

    switch (port)
    {
        case 0:
            // Write the new config to the buffer.
            switch (state)
            {
                case 0:
                    handle->Port0_PinState_Buffer &= ~(1 << pin);
                    break;
                case 1:
                    handle->Port0_PinState_Buffer |= (1 << pin);
                    break;
                default:
                    return;
            }
            break;
        case 1:
            // Write the new config to the buffer.
            switch (state)
            {
                case 0:
                    handle->Port1_PinState_Buffer &= ~(1 << pin);
                    break;
                case 1:
                    handle->Port1_PinState_Buffer |= (1 << pin);
                    break;
                default:
                    return;
            }
            break;
        default:
            return;
    }

    // Update the chip with the newly written buffer.
    _PCA9555_UpdateStateRegisterFromBuf(handle, port);
}

void PCA9555_WritePort(PCA9555_Handle_t *handle, uint8_t port, uint8_t state_byte)
{
    // Update the whole buffer to the new value.
    switch (port)
    {
        case 0:
            handle->Port0_PinState_Buffer = state_byte;
            break;
        case 1:
            handle->Port1_PinState_Buffer = state_byte;
            break;
        default:
            return;
    }

    // Update the chip with the newly written buffer.
    _PCA9555_UpdateStateRegisterFromBuf(handle, port);
}

uint8_t PCA9555_ReadPin(PCA9555_Handle_t *handle, uint8_t port, uint8_t pin)
{
    if (pin > 7)
        return 0U;

    uint8_t buf;

    switch (port)
    {
        case 0:
            buf = _PCA9555_ReadRegister(handle, PCA9555_REGISTER_PORT0_INPUT);
            break;
        case 1:
            buf = _PCA9555_ReadRegister(handle, PCA9555_REGISTER_PORT1_INPUT);
            break;
        default:
            return 0U;
    }

    return ((buf >> pin) & 0x01);
}

uint8_t PCA9555_ReadPort(PCA9555_Handle_t *handle, uint8_t port)
{
    uint8_t buf;

    switch (port)
    {
        case 0:
            buf = _PCA9555_ReadRegister(handle, PCA9555_REGISTER_PORT0_INPUT);
            break;
        case 1:
            buf = _PCA9555_ReadRegister(handle, PCA9555_REGISTER_PORT1_INPUT);
            break;
        default:
            return 0U;
    }

    return buf;
}

void PCA9555_ConfigPinMode(PCA9555_Handle_t *handle, uint8_t port, uint8_t pin, uint8_t pin_mode)
{
    // Check for the parameters to be in range.
    if (!(port <= 1 && pin <= 7))
        return;

    switch (port)
    {
        case 0:
            // Write the new config to the buffer.
            switch (pin_mode)
            {
                case 0:
                    handle->Port0_PinMode_Buffer &= ~(1 << pin);
                    break;
                case 1:
                    handle->Port0_PinMode_Buffer |= (1 << pin);
                    break;
                default:
                    return;
            }
            break;
        case 1:
            // Write the new config to the buffer.
            switch (pin_mode)
            {
                case 0:
                    handle->Port1_PinMode_Buffer &= ~(1 << pin);
                    break;
                case 1:
                    handle->Port1_PinMode_Buffer |= (1 << pin);
                    break;
                default:
                    return;
            }
            break;
        default:
            return;
    }

    // Update the chip with the newly written buffer.
    _PCA9555_UpdateCfgRegisterFromBuf(handle, port);
}

void PCA9555_ConfigPortPinMode(PCA9555_Handle_t *handle, uint8_t port, uint8_t pin_mode_byte)
{
    // Update the whole buffer to the new value.
    switch (port)
    {
        case 0:
            handle->Port0_PinMode_Buffer = pin_mode_byte;
            break;
        case 1:
            handle->Port1_PinMode_Buffer = pin_mode_byte;
            break;
        default:
            return;
    }

    // Update the chip with the newly written buffer.
    _PCA9555_UpdateCfgRegisterFromBuf(handle, port);
}

void PCA9555_ConfigPinInputInversion(PCA9555_Handle_t *handle, uint8_t port, uint8_t pin, uint8_t inv_mode)
{
    // Check for the parameters to be in range.
    if (!(port <= 1 && pin <= 7))
        return;

    switch (port)
    {
        case 0:
            // Write the new config to the buffer.
            switch (inv_mode)
            {
                case 0:
                    handle->Port0_InputInversion_Buffer &= ~(1 << pin);
                    break;
                case 1:
                    handle->Port0_InputInversion_Buffer |= (1 << pin);
                    break;
                default:
                    return;
            }
            break;
        case 1:
            // Write the new config to the buffer.
            switch (inv_mode)
            {
                case 0:
                    handle->Port1_InputInversion_Buffer &= ~(1 << pin);
                    break;
                case 1:
                    handle->Port1_InputInversion_Buffer |= (1 << pin);
                    break;
                default:
                    return;
            }
            break;
        default:
            return;
    }

    // Update the chip with the newly written buffer.
    _PCA9555_UpdateInvRegisterFromBuf(handle, port);
}

void PCA9555_ConfigPortPinInputInversion(PCA9555_Handle_t *handle, uint8_t port, uint8_t inv_mode_byte)
{
    // Update the whole buffer to the new value.
    switch (port)
    {
        case 0:
            handle->Port0_InputInversion_Buffer = inv_mode_byte;
            break;
        case 1:
            handle->Port1_InputInversion_Buffer = inv_mode_byte;
            break;
        default:
            return;
    }

    // Update the chip with the newly written buffer.
    _PCA9555_UpdateInvRegisterFromBuf(handle, port);
}

static void _PCA9555_WriteRegister(PCA9555_Handle_t *handle, uint8_t reg, uint8_t data)
{
    uint8_t buf[2];
    buf[0] = reg;
    buf[1] = data;

    _lib_cfg.I2C_Write(handle->I2C_Port_ID, handle->I2C_Device_Address << 1, 2, buf);
}

static uint8_t _PCA9555_ReadRegister(PCA9555_Handle_t *handle, uint8_t reg)
{
    _lib_cfg.I2C_Write(handle->I2C_Port_ID, (handle->I2C_Device_Address << 1) + 1, 1, &reg);
    return _lib_cfg.I2C_Read(handle->I2C_Port_ID, handle->I2C_Device_Address, 1)[0];
}

static void _PCA9555_UpdateCfgRegisterFromBuf(PCA9555_Handle_t *handle, uint8_t port)
{
    switch (port)
    {
        case 0:
            _PCA9555_WriteRegister(handle, PCA9555_REGISTER_PORT0_CONFIG, handle->Port0_PinMode_Buffer);
            break;
        case 1:
            _PCA9555_WriteRegister(handle, PCA9555_REGISTER_PORT1_CONFIG, handle->Port1_PinMode_Buffer);
            break;
        default:
            return;
    }
}

static void _PCA9555_UpdateInvRegisterFromBuf(PCA9555_Handle_t *handle, uint8_t port)
{
    switch (port)
    {
        case 0:
            _PCA9555_WriteRegister(handle, PCA9555_REGISTER_PORT0_POLINV, handle->Port0_InputInversion_Buffer);
            break;
        case 1:
            _PCA9555_WriteRegister(handle, PCA9555_REGISTER_PORT1_POLINV, handle->Port1_InputInversion_Buffer);
            break;
        default:
            return;
    }
}

static void _PCA9555_UpdateStateRegisterFromBuf(PCA9555_Handle_t *handle, uint8_t port)
{
    switch (port)
    {
        case 0:
            _PCA9555_WriteRegister(handle, PCA9555_REGISTER_PORT0_OUTPUT, handle->Port0_PinState_Buffer);
            break;
        case 1:
            _PCA9555_WriteRegister(handle, PCA9555_REGISTER_PORT1_OUTPUT, handle->Port1_PinState_Buffer);
            break;
        default:
            return;
    }
}
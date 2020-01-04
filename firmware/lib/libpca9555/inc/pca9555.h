/*******************************************************************************
* \file pca9555.h
* \date 03.01.2020
********************************************************************************
* \author Johannes Berndorfer (berndoJ)
* \copyright Copyright (c) 2020 by Johannes Berndorfer (berndoJ)
********************************************************************************
* \brief Main interface for this library (libpca9555).
*******************************************************************************/

#if !defined(__PCA9555_H)
#define __PCA9555_H

#include <stdint.h>

#define PCA9555_REGISTER_PORT0_INPUT        0x00
#define PCA9555_REGISTER_PORT1_INPUT        0x01
#define PCA9555_REGISTER_PORT0_OUTPUT       0x02
#define PCA9555_REGISTER_PORT1_OUTPUT       0x03
#define PCA9555_REGISTER_PORT0_POLINV       0x04
#define PCA9555_REGISTER_PORT1_POLINV       0x05
#define PCA9555_REGISTER_PORT0_CONFIG       0x06
#define PCA9555_REGISTER_PORT1_CONFIG       0x07

/**
 * \brief A structure to contain all necessary configuration for this library.
 */
typedef struct 
{
    void (*I2C_Write)(uint8_t port_id, uint8_t device_address, uint8_t len, uint8_t *buf); /**< Function pointer to write the specified data to the specified I2C bus / device. */
    uint8_t *(*I2C_Read)(uint8_t port_id, uint8_t device_address, uint8_t len); /**< Function pointer to read the specified data to the specified I2C bus / device. */
} PCA9555_LibConfig_t;

/**
 * \brief A structure to represent one PCA9555 chip on the host platform. This structure is used to enable this library
 *        to work with more than one PCA9555 chip per processor.
 */
typedef struct
{
    uint8_t I2C_Port_ID; /**< Number of the I2C port the PCA9555 device sits on. This ID is passed to the I2C write and read function in order to support different PCA9555 devices on different I2C busses of the microcontroller. */
    uint8_t I2C_Device_Address; /**< The 7-bit device address of the PCA9555 device this handle refers to.*/
    uint8_t Port0_PinMode_Buffer; /**< Buffer for the pin mode configuration of port 0.*/
    uint8_t Port0_InputInversion_Buffer; /**< Buffer for the input state inversion configuration of port 0.*/
    uint8_t Port0_PinState_Buffer; /**< Buffer for the current output pin states of port 0.*/
    uint8_t Port1_PinMode_Buffer; /**< Buffer for the pin mode configuration of port 1.*/
    uint8_t Port1_InputInversion_Buffer; /**< Buffer for the input state inversion configuration of port 1.*/
    uint8_t Port1_PinState_Buffer; /**< Buffer for the current output pin states of port 1.*/
} PCA9555_Handle_t;

/**
 * \brief Initialises the library with the given configuration struct.
 * \param cfg Pointer to the configuration struct this library should be initialised with. The implementer of this
 *            library has to provide two functions - an I2C read and a write function. These are used as an interface
 *            between this library and the HAL-libraries of the application this library is implemented on.
 */
void PCA9555_LibInit(PCA9555_LibConfig_t *cfg);

/**
 * \brief Writes the state of the given pin on the given port.
 * \param handle The PCA9555 device handle.
 * \param port The port the pin which should be written is located on. (0 or 1)
 * \param pin The pin which should be written. (0 to 7)
 * \param state The new state of the pin. (0 or 1 = LOW or HIGH)
 */
void PCA9555_WritePin(PCA9555_Handle_t *handle, uint8_t port, uint8_t pin, uint8_t state);

/**
 * \brief Writes the state of an entire port.
 * \param handle The PCA9555 device handle.
 * \param port The port which should be updated. (0 or 1)
 * \param state_byte The new state of the port. The LSB represents pin 0, the MSB represents pin 7 of the port. Each bit,
 *                   either 0 or 1 = LOW or HIGH, of this byte is the new state of the corresponding pin on the port.
 */
void PCA9555_WritePort(PCA9555_Handle_t *handle, uint8_t port, uint8_t state_byte);

/**
 * \brief Reads the state of a given pin on the given port.
 * \param handle The PCA9555 device handle.
 * \param port The port on which the pin which should be read is located on. (0 or 1)
 * \param pin The pin of which the state should be read. (0 to 7)
 * \return The state of the given pin on the given port. (0 or 1 = LOW or HIGH)
 */
uint8_t PCA9555_ReadPin(PCA9555_Handle_t *handle, uint8_t port, uint8_t pin);

/**
 * \brief Reads the state of an entire port.
 * \param handle The PCA9555 device handle.
 * \param port The port of which the state should be read. (0 or 1)
 * \return The state of the entire port given. The LSB represents pin 0, the MSB represents pin 7 of the port. Each bit,
 *         either 0 or 1 = LOW or HIGH, of this byte is the read state of the corresponding pin on the port.
 */
uint8_t PCA9555_ReadPort(PCA9555_Handle_t *handle, uint8_t port);

/**
 * \brief Sets the pin mode configuration for a given pin on a given port.
 * \param handle The PCA9555 device handle.
 * \param port The port on which the pin which should be configured is located on. (0 or 1)
 * \param pin The pin which should be configured. (0 to 7)
 * \param pin_mode The the new pin mode of the pin given. 1 = Input, 0 = Output. (0 or 1)
 */
void PCA9555_ConfigPinMode(PCA9555_Handle_t *handle, uint8_t port, uint8_t pin, uint8_t pin_mode);

/**
 * \brief Sets the pin mode configuration for a given port.
 * \param handle The PCA9555 device handle.
 * \param port The port of which the pin modes should be configured. (0 or 1)
 * \param pin_mode_byte The pin mode config of the entire port. The LSB represents pin 0, the MSB represents pin 7 of
 *                      the port. 1 = Input, 0 = Output.
 */
void PCA9555_ConfigPortPinMode(PCA9555_Handle_t *handle, uint8_t port, uint8_t pin_mode_byte);

/**
 * \brief Sets the input bit inversion mode for a given pin on the given port. The input inversion mode selects if the
 *        input of a given pin on a given port should be inverted before latched into the input register.
 * \param handle The PCA9555 device handle.
 * \param port The port on which the pin to configure is located on. (0 or 1)
 * \param pin The pin to configure the input bit inversion mode for. (0 to 7)
 * \param inv_mode The new inversion mode for the pin given. 1 = Inverted, 0 = Non-Inverted. (0 or 1)
 */
void PCA9555_ConfigPinInputInversion(PCA9555_Handle_t *handle, uint8_t port, uint8_t pin, uint8_t inv_mode);

/**
 * \brief Sets the input bit inversion mode for a given port. The input inversion mode selects if the input of a given
 *        pin on the given port should be inverted before latched into the input register. 
 * \param handle The PCA9555 device handle.
 * \param port The port which should be configured. (0 or 1)
 * \param inv_mode_byte The inversion mode of each bit on the given port. The LSB represents pin 0, the
 *        MSB represents pin 7 of the port. 1 = Inverted, 0 = Non-Inverted.
 */
void PCA9555_ConfigPortPinInputInversion(PCA9555_Handle_t *handle, uint8_t port, uint8_t inv_mode_byte);


#endif // __PCA9555_H
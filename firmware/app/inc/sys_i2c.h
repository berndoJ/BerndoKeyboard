/*******************************************************************************
* \file sys_i2c.h
* \date 04.01.2020
********************************************************************************
* \author Johannes Berndorfer (berndoJ)
* \copyright Copyright (c) 2020 by Johannes Berndorfer (berndoJ)
********************************************************************************
* \brief Interface module for all I2C ports on the host processor.
*******************************************************************************/

#if !defined(__SYS_I2C_H)
#define __SYS_I2C_H

#include "sys_def.h"
#include "stm32f1xx_hal.h"

#define SYS_I2C1_CLOCK_FREQ         100000U
#define SYS_I2C1_HOST_ADDRESS_1     0x00
#define SYS_I2C1_HOST_ADDRESS_2     0x01

/**
 * \brief Handle for the I2C1 port.
 */
extern I2C_HandleTypeDef *hw_i2c1_handle;

/**
 * \brief Initialisation status for the I2C1 port.
 */
extern SYS_InitStatus_t hw_i2c1_init_status;

/**
 * \brief Initialises all I2C ports on the host processor.
 */
void SYS_I2C_Init(void);

#endif // __SYS_I2C_H
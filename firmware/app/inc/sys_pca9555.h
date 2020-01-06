/*******************************************************************************
* \file sys_pca9555.h
* \date 04.01.2020
********************************************************************************
* \author Johannes Berndorfer (berndoJ)
* \copyright Copyright (c) 2020 by Johannes Berndorfer (berndoJ)
********************************************************************************
* \brief Interface module for libpca9555. Manages all interactions with the
*        PCA9555 port expander devices on the host platform.
*******************************************************************************/

#if !defined(__SYS_PCA9555_H)
#define __SYS_PCA9555_H

#include "sys_def.h"
#include "pca9555.h"

#define SYS_PCA9555_I2C_TIMEOUT_MS  10

/**
 * \brief Handle for the PORTEX0 PCA9555 device on the host platform.
 */ 
extern PCA9555_Handle_t* hw_portex0_handle;

/**
 * \brief Initialisation status for the PORTEX0 PCA9555 device on the host platform.
 */
extern SYS_InitStatus_t hw_portex0_init_status;

/**
 * \brief Handle for the PORTEX1 PCA9555 device on the host platform.
 */ 
extern PCA9555_Handle_t* hw_portex1_handle;

/**
 * \brief Initialisation status for the PORTEX1 PCA9555 device on the host platform.
 */
extern SYS_InitStatus_t hw_portex1_init_status;

/**
 * \brief Initialises the PCA9555 library as well as the 
 */
void SYS_PCA9555_Init(void);

#endif // __SYS_PCA9555_H
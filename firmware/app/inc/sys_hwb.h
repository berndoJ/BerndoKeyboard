/*******************************************************************************
* \file sys_hwb.h
* \date 04.01.2020
********************************************************************************
* \author Johannes Berndorfer (berndoJ)
* \copyright Copyright (c) 2020 by Johannes Berndorfer (berndoJ)
********************************************************************************
* \brief Handler for the platform hw-bits. The hw-bits are a set of five bits,
*        set by resistors on the PCB of the controller module. They can be used
*        to pass the software information about the current hardware of the host
*        platform, or to enable special software features (such as debug mode).
*******************************************************************************/

#if !defined(__SYS_HWB_H)
#define __SYS_HWB_H

#include <stdint.h>

/**
 * \brief Gets the specified hwbit.
 */
#define SYS_HWBIT(bit)    ((sys_hwbyte >> bit) & 0x01)

/**
 * \brief Contains the read hw-bits.
 */
extern uint8_t sys_hwbyte;

/**
 * \brief Reads the hw-bits of the host platform and populates the
 *        \ref sys_hwbyte variable.
 */
void SYS_HWB_Read(void);

#endif // __SYS_HWB_H
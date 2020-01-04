/*******************************************************************************
* \file sys_clock.h
* \date 03.01.2020
********************************************************************************
* \author Johannes Berndorfer (berndoJ)
* \copyright Copyright (c) 2020 by Johannes Berndorfer (berndoJ)
********************************************************************************
* \brief Configures and initialises the clock architecture (PLLs, MUXs, ...) of
*        the microcontroller.
*******************************************************************************/

#if !defined(__SYS_CLOCK_H)
#define __SYS_CLOCK_H

/**
 * \brief Initialises the clock architecture of the microcontroller. This sets
 *        up all necessary registers which control the PLLs and MUXs within the
 *        clock architecture of the microcontroller. This will also set up the 
 *        config of which clock source to use for the main CPU clock and the RTC
 *        clock.
 */
void SYS_Clock_Init(void);

#endif // __SYS_CLOCK_H
/*******************************************************************************
* \file mcu_init.h
* \date 03.01.2020
********************************************************************************
* \brief Initialises the microcontroller before the main application starts.
*******************************************************************************/

#if !defined(__MCU_INIT_H)
#define __MCU_INIT_H

/*
#### Clock Frequency Configuration ####
*/

#if !defined(HSE_VALUE)
#   define HSE_VALUE        16000000U // HSE Crystal frequency (in Hz) -> 16MHz
#endif

#if !defined(HSI_VALUE)
#   define HSI_VALUE        8000000U // HSI Oscillator (Internal RC oscillator) frequency (in Hz) -> 8MHz
#endif

/*
#### Memory Configuration ####
*/

#define VECT_TAB_OFFSET     0x00000000U // Vector table offset. This value must be a multiple of 0x200


#endif // __MCU_INIT_H
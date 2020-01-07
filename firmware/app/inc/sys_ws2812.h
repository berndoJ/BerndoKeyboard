/*******************************************************************************
* \file sys_ws2812.h
* \date 06.01.2020
********************************************************************************
* \author Johannes Berndorfer (berndoJ)
* \copyright Copyright (c) 2020 by Johannes Berndorfer (berndoJ)
********************************************************************************
* \brief Interface module for libneopixel32. Manages all interactions with the
*        WS2812 LEDs and configures the library correctly. This module also
*        provides the correct callbacks for the library.
*******************************************************************************/

#if !defined(__SYS_WS2812_H)
#define __SYS_WS2812_H

#include "sys_def.h"
#include "neopixel32.h"

extern NP32_Instance_t *hw_ws2812_handle;
extern SYS_InitStatus_t hw_ws2812_init_status;

/**
 * \brief Initialises libneopixel32.
 */
void SYS_WS2812_Init(void);

#endif // __SYS_WS2812_H
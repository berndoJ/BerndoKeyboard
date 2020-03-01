/*----------------------------------------------------------------
 * FILENAME :		infinikeys_stm32cube_usb_if.h
 * PROJECT :		infinikeys-arm32
 * AUTHOR :			Johannes Berndorfer
 * BREIF :			STM32 CUBE Custom HID USB Interface
 * 
 * Copyright (c) 2019 Johannes Berndorfer. All rights reserved.
 * ---------------------------------------------------------------
 * DATE CREATED :	22.09.2019	
 * VERSION :		1.0
 * ---------------------------------------------------------------
 * DESCRIPTION :
 * 		Custom interface module for the STM32 CUBE software
 * 		platform. This module is used to link the infinikeys
 * 		USB software to the HAL USB software of the STM32 CUBE
 * 		library.
 * 
 *--------------------------------------------------------------*/

#ifndef INFINIKEYS_USB_IF_STM32CUBE_H_
#define INFINIKEYS_USB_IF_STM32CUBE_H_

/* --------------------------------------------------------------
 * INCLUDES
 * ------------------------------------------------------------*/

#include <stdint.h>

#include "infinikeys_stm32cube_usb_hid.h"

/* --------------------------------------------------------------
 * VARIABLE DECLARATIONS
 * ------------------------------------------------------------*/

/*
 * Description
 * IK HID interface callback.
 */
extern IK_HID_DeviceInterface_t IK_USBD_HID_Itf;

#endif /* INFINIKEYS_USB_IF_STM32CUBE_H_ */


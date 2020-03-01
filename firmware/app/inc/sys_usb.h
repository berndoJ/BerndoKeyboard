/*******************************************************************************
* \file sys_usb.h
* \date 01.03.2020
********************************************************************************
* \author Johannes Berndorfer (berndoJ)
* \copyright Copyright (c) 2020 by Johannes Berndorfer (berndoJ)
********************************************************************************
* \brief Interface module for the implementation of an USB device.
*******************************************************************************/

#if !defined(__SYS_USB_H)
#define __SYS_USB_H

#include "sys_def.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include <stdint.h>

#define _IK_USB_DESCRIPTOR_DEV_QUALIF_SIZE 10

extern uint8_t _IK_USB_DESCRIPTOR_DEV_QUALIF[_IK_USB_DESCRIPTOR_DEV_QUALIF_SIZE];

extern USBD_HandleTypeDef *hw_usbd_handle;

void SYS_USB_Init(void);
void SYS_USB_DeInit(void);

#endif // __SYS_USB_H
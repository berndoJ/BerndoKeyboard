/*******************************************************************************
* \file infinikeys_usb.h
* \date 23.01.2020
********************************************************************************
* \author Johannes Berndorfer (berndoJ)
* \copyright Copyright (c) 2020 Johannes Berndorfer (berndoJ)
********************************************************************************
* \brief Contains all definitions of USB descriptors for the USB device.
*******************************************************************************/

#if !defined(__INFINIKEYS_USB_DESC_H)
#define __INFINIKEYS_USB_DESC_H

#include "infinikeys_def.h"
#include "infinikeys_usb.h"

#define IK_USB_DESCRIPTOR_DEVICE_SIZE           18U
#define IK_USB_DESCRIPTOR_CONFIG_SIZE           41U
#define IK_USB_DESCRIPTOR_HID_REPORT_SIZE       65U
#define IK_USB_DESCRIPTOR_LANGID_SIZE           4U

extern uint8_t IK_USB_DESCRIPTOR_DEVICE[IK_USB_DESCRIPTOR_DEVICE_SIZE];
extern uint8_t IK_USB_DESCRIPTOR_CONFIG[IK_USB_DESCRIPTOR_CONFIG_SIZE];
extern uint8_t IK_USB_DESCRIPTOR_HID_REPORT[IK_USB_DESCRIPTOR_HID_REPORT_SIZE];
extern uint8_t IK_USB_DESCRIPTOR_LANGID[IK_USB_DESCRIPTOR_LANGID_SIZE];

#endif // __INFINIKEYS_USB_DESC_H
/*----------------------------------------------------------------
 * FILENAME :		infinikeys_stm32cube_usb_if.c
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

#include "infinikeys_stm32cube_usb_if.h"

#include "infinikeys_usb_desc.h"

#include "sys_ws2812.h"

/* --------------------------------------------------------------
 * FUNCTION DECLARATIONS
 * ------------------------------------------------------------*/

static int8_t IK_USBD_HID_Init(void);
static int8_t IK_USBD_HID_DeInit(void);
static int8_t IK_USBD_HID_OutEvent(uint8_t* report_buffer);

/* --------------------------------------------------------------
 * VARIABLE DEFINITIONS
 * ------------------------------------------------------------*/

IK_HID_DeviceInterface_t IK_USBD_HID_Itf =
{
	IK_USB_DESCRIPTOR_HID_REPORT,
	IK_USBD_HID_Init,
	IK_USBD_HID_DeInit,
	IK_USBD_HID_OutEvent
};

/* --------------------------------------------------------------
 * FUNCTION DEFINITIONS
 * ------------------------------------------------------------*/

/*
 * Description
 * USB HID Interface init.
 */
static int8_t IK_USBD_HID_Init(void)
{
	return USBD_OK;
}

/*
 * Description
 * USB HID Interface de-init.
 */
static int8_t IK_USBD_HID_DeInit(void)
{
	return USBD_OK;
}

/*
 * Description
 * USB HID Interface OUTEvent (Microcontroller Data-IN) Handler.
 */
static int8_t IK_USBD_HID_OutEvent(uint8_t* report_buffer)
{
    uint8_t report_id, kleds;

    report_id = report_buffer[0];

    switch (report_id)
    {
        case 1:
        case 2:
            // Keyboard HID LED-status report.
            kleds = report_buffer[1];
            if (hw_ws2812_init_status == SYS_INIT_STATUS_INITIALISED)
            {
                NP32_SetLED_RGB(hw_ws2812_handle, KBLED_NUMLOCK_INDEX, ((kleds & 0x01) == 1 ? KBLED_DEFAULT_COLOR_RGB : KBLED_COLOR_BLACK));
                NP32_SetLED_RGB(hw_ws2812_handle, KBLED_CAPSLOCK_INDEX, (((kleds >> 1) & 0x01) == 1 ? KBLED_DEFAULT_COLOR_RGB : KBLED_COLOR_BLACK));
                NP32_SetLED_RGB(hw_ws2812_handle, KBLED_SCROLLLOCK_INDEX, (((kleds >> 2) & 0x01) == 1 ? KBLED_DEFAULT_COLOR_RGB : KBLED_COLOR_BLACK));
                NP32_Update(hw_ws2812_handle);
            }
            break;
        default:
            break;
    }

	//IK_USBIF_RecieveHIDReportCB(report_buffer, IK_HID_OUT_REPORT_BUFFER_SIZE);
	return USBD_OK;
}


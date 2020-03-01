/*----------------------------------------------------------------
 * FILENAME :		infinikeys_stm32cube_usb_hid.h
 * PROJECT :		infinikeys-arm32
 * AUTHOR :			Johannes Berndorfer
 * BREIF :			STM32 CUBE USB HID device module.
 * 
 * Copyright (c) 2019 Johannes Berndorfer. All rights reserved.
 * ---------------------------------------------------------------
 * DATE CREATED :	21.09.2019	
 * VERSION :		1.0
 * ---------------------------------------------------------------
 * DESCRIPTION :
 * 		USB HID device module for the STM32 CUBE platform.
 * 
 *--------------------------------------------------------------*/

#ifndef INFINIKEYS_STM32CUBE_USB_HID_H_
#define INFINIKEYS_STM32CUBE_USB_HID_H_

/* --------------------------------------------------------------
 * INCLUDES
 * ------------------------------------------------------------*/

#include <stdint.h>

#include "usbd_ioreq.h"

/* --------------------------------------------------------------
 * DEFINITIONS
 * ------------------------------------------------------------*/

// HID USB endpoint addresses and sizes.
#define IK_HID_EPIN_ADDR				0x81
#define IK_HID_EPIN_SIZE				0x0a
#define IK_HID_EPIN_INTERVAL_MS			5
#define IK_HID_EPOUT_ADDR				0x01
#define IK_HID_EPOUT_SIZE				0x0a
#define IK_HID_EPOUT_INTERVAL_MS		5

#define IK_HID_REQUEST_SET_PROTOCOL		0x0b
#define IK_HID_REQUEST_GET_PROTOCOL		0x03
#define IK_HID_REQUEST_SET_IDLE			0x0a
#define IK_HID_REQUEST_GET_IDLE			0x02
#define IK_HID_REQUEST_SET_REPORT		0x09
#define IK_HID_REQUEST_GET_REPORT		0x01

// Descriptor config
#define IK_HID_REPORT_DESC_SIZE			91
#define IK_USB_CONFIG_DESC_SIZE			41
#define IK_HID_DESC_SIZE				9
#define IK_HID_DESC_TYPE				0x21
#define IK_HID_REPORT_DESC_TYPE			0x22

// HID Report
#define IK_HID_OUT_REPORT_BUFFER_SIZE	0x50
#define IK_HID_REPORT_ID_KEYBOARD		0x01
#define IK_HID_REPORT_ID_IKCONFIG		0x02

// USBD class pointer macro
#define IK_HID_CLASS_P					&IK_HID_Class

/* --------------------------------------------------------------
 * STRUCTS, ENUMS AND TYPEDEFS
 * ------------------------------------------------------------*/

typedef enum {
	IK_HID_IDLE = 0,
	IK_HID_BUSY = 1
} IK_HID_State_t;

typedef struct {
	uint8_t* Report;
	int8_t (*Init) (void);
	int8_t (*DeInit) (void);
	int8_t (*OutEvent) (uint8_t*);
} IK_HID_DeviceInterface_t;

typedef struct {
	uint8_t ReportBuffer[IK_HID_OUT_REPORT_BUFFER_SIZE];
	uint32_t Protocol;
	uint32_t IdleState;
	uint32_t AltSetting;
	uint32_t IsReportAvailable;
	IK_HID_State_t State;
} IK_HID_DeviceHandle_t;

/* --------------------------------------------------------------
 * VARIABLE DECLARATIONS
 * ------------------------------------------------------------*/

extern USBD_ClassTypeDef IK_HID_Class;

/* --------------------------------------------------------------
 * FUNCTION DECLARATIONS
 * ------------------------------------------------------------*/

/*
 * Description
 * Sends a new HID report to the USB host.
 *
 * Parameters
 * - USBD_HandleTypeDef* device: The device instance.
 * - uint8_t* report: The report to send.
 * - uint16_t report_length: The report's size.
 */
uint8_t IK_STM32CUBE_USBD_HID_SendReport(USBD_HandleTypeDef* device, uint8_t* report, uint16_t report_length);

/*
 * Description
 * Registers a new interface.
 *
 * Parameters:
 * - USBD_HandleTypeDef* device: The device instance.
 * - IK_HID_DeviceInterface_t* interface: The new interface to register.
 */
uint8_t IK_STM32CUBE_USBD_HID_RegisterInterface(USBD_HandleTypeDef* device, IK_HID_DeviceInterface_t* interface);

#endif /* INFINIKEYS_STM32CUBE_USB_HID_H_ */

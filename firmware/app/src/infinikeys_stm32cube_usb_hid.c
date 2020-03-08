/*----------------------------------------------------------------
 * FILENAME :		infinikeys_stm32cube_usb_hid.c
 * PROJECT :		JKP-Firmware-InfiniKeys-v1.x
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

/* --------------------------------------------------------------
 * INCLUDES
 * ------------------------------------------------------------*/

#include "infinikeys_stm32cube_usb_hid.h"

//#include "infinikeys_stm32cube_usb_descriptors.h"
#include "usbd_desc.h"
#include "usbd_ctlreq.h"
#include "infinikeys_usb_desc.h"
#include "sys_usb.h"
#include "console.h"

/* --------------------------------------------------------------
 * PRIVATE FUNCTION DECLARATIONS
 * ------------------------------------------------------------*/

static uint8_t IK_STM32CUBE_USBD_HID_Init(USBD_HandleTypeDef* device, uint8_t config_index);
static uint8_t IK_STM32CUBE_USBD_HID_DeInit(USBD_HandleTypeDef* device, uint8_t config_index);
static uint8_t IK_STM32CUBE_USBD_HID_Setup(USBD_HandleTypeDef* device, USBD_SetupReqTypedef* request);
static uint8_t* IK_STM32CUBE_USBD_HID_GetConfigDescriptor(uint16_t* length);
static uint8_t* IK_STM32CUBE_USBD_HID_GetDeviceQualifierDescriptor(uint16_t* length);
static uint8_t IK_STM32CUBE_USBD_HID_DataIn(USBD_HandleTypeDef* device, uint8_t endpoint_num);
static uint8_t IK_STM32CUBE_USBD_HID_DataOut(USBD_HandleTypeDef* device, uint8_t endpoint_num);
static uint8_t IK_STM32CUBE_USBD_HID_EP0RecieveReady(USBD_HandleTypeDef* device);

/* --------------------------------------------------------------
 * VARIABLE DEFINITIONS
 * ------------------------------------------------------------*/

// Infinikeys USBD class
USBD_ClassTypeDef IK_HID_Class =
{
	IK_STM32CUBE_USBD_HID_Init,
	IK_STM32CUBE_USBD_HID_DeInit,
	IK_STM32CUBE_USBD_HID_Setup,
	NULL, // EP0_TxSent
	IK_STM32CUBE_USBD_HID_EP0RecieveReady,
	IK_STM32CUBE_USBD_HID_DataIn,
	IK_STM32CUBE_USBD_HID_DataOut,
	NULL, // SOF
	NULL, // IsoINIncomplete
	NULL, // IsoOUTIncomplete
	IK_STM32CUBE_USBD_HID_GetConfigDescriptor,
	IK_STM32CUBE_USBD_HID_GetConfigDescriptor,
	IK_STM32CUBE_USBD_HID_GetConfigDescriptor,
	IK_STM32CUBE_USBD_HID_GetDeviceQualifierDescriptor
};

/* --------------------------------------------------------------
 * FUNCTION DEFINITIONS
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
uint8_t IK_STM32CUBE_USBD_HID_SendReport(USBD_HandleTypeDef* device, uint8_t* report, uint16_t report_length)
{
	IK_HID_DeviceHandle_t* ikdev_handle = (IK_HID_DeviceHandle_t*) device->pClassData;

	if (device->dev_state == USBD_STATE_CONFIGURED)
	{
		if (ikdev_handle->State == IK_HID_IDLE)
		{
			ikdev_handle->State = IK_HID_BUSY;
			USBD_LL_Transmit(device, IK_HID_EPIN_ADDR, report, report_length);
		}
	}

	return USBD_OK;
}

/*
 * Description
 * Registers a new interface.
 *
 * Parameters:
 * - USBD_HandleTypeDef* device: The device instance.
 * - IK_HID_DeviceInterface_t* interface: The new interface to register.
 */
uint8_t IK_STM32CUBE_USBD_HID_RegisterInterface(USBD_HandleTypeDef* device, IK_HID_DeviceInterface_t* interface)
{
	if (interface == NULL)
		return USBD_FAIL;

	device->pUserData = interface;

	return USBD_OK;
}

/* --------------------------------------------------------------
 * PRIVATE FUNCTION DEFINITIONS
 * ------------------------------------------------------------*/

/*
 * Description
 * Initialises the USB HID interface.
 *
 * Parameters
 * - USBD_HandleTypeDef* device: The device instance.
 * - uint8_t config_index: The configuration index.
 */
static uint8_t IK_STM32CUBE_USBD_HID_Init(USBD_HandleTypeDef* device, uint8_t config_index)
{
	IK_HID_DeviceHandle_t* ikdev_handle;

	USBD_LL_OpenEP(device, IK_HID_EPIN_ADDR, USBD_EP_TYPE_INTR, IK_HID_EPIN_SIZE);
	USBD_LL_OpenEP(device, IK_HID_EPOUT_ADDR, USBD_EP_TYPE_INTR, IK_HID_EPOUT_SIZE);

	device->pClassData = USBD_malloc(sizeof(IK_HID_DeviceHandle_t));

	if (device->pClassData == NULL)
		return USBD_FAIL;

	ikdev_handle = (IK_HID_DeviceHandle_t*) device->pClassData;
	ikdev_handle->State = IK_HID_IDLE;
	((IK_HID_DeviceInterface_t*) device->pUserData)->Init();

	USBD_LL_PrepareReceive(device, IK_HID_EPOUT_ADDR, ikdev_handle->ReportBuffer, IK_HID_OUT_REPORT_BUFFER_SIZE);

	return USBD_OK;
}

/*
 * Description
 * De-initialises the USB HID interface.
 *
 * Parameters
 * - USBD_HandleTypeDef* device: The device instance.
 * - uint8_t config_index: The configuration index.
 */
static uint8_t IK_STM32CUBE_USBD_HID_DeInit(USBD_HandleTypeDef* device, uint8_t config_index)
{
	USBD_LL_CloseEP(device, IK_HID_EPIN_ADDR);
	USBD_LL_CloseEP(device, IK_HID_EPOUT_ADDR);

	if (device->pClassData != NULL)
	{
		((IK_HID_DeviceInterface_t*) device->pUserData)->DeInit();
		USBD_free(device->pClassData);
		device->pClassData = NULL;
	}

	return USBD_OK;
}

/*
 * Description
 * Handle HID specific requests.
 *
 * Parameters:
 * - USBD_HandleTypeDef* device: The device instance.
 * - USBD_SetupReqTypedef* request: USB setup request.
 */
static uint8_t IK_STM32CUBE_USBD_HID_Setup(USBD_HandleTypeDef* device, USBD_SetupReqTypedef* request)
{
	IK_HID_DeviceHandle_t* ikdev_handle = (IK_HID_DeviceHandle_t*) device->pClassData;
	uint8_t* buffer = NULL;
	uint16_t length = 0;

	switch (request->bmRequest & USB_REQ_TYPE_MASK)
	{
	case USB_REQ_TYPE_STANDARD:
		switch (request->bRequest)
		{
		case USB_REQ_GET_DESCRIPTOR:
			if ((request->wValue >> 8) == IK_HID_REPORT_DESC_TYPE)
			{
				length = MIN(IK_HID_REPORT_DESC_SIZE, request->wLength);
				buffer = ((IK_HID_DeviceInterface_t*) device->pUserData)->Report;
			}
			else if ((request->wValue >> 8) == IK_HID_DESC_TYPE)
			{
				buffer = IK_USB_DESCRIPTOR_HID_REPORT;
				length = MIN(IK_HID_DESC_SIZE, request->wLength);
			}
			USBD_CtlSendData(device, buffer, length);
			break;
		case USB_REQ_GET_INTERFACE:
			USBD_CtlSendData(device, (uint8_t*)&(ikdev_handle->AltSetting), 1);
			break;
		case USB_REQ_SET_INTERFACE:
			ikdev_handle->AltSetting = (uint8_t)(request->wValue);
			break;
		}
		break;
	case USB_REQ_TYPE_CLASS:
		switch (request->bRequest)
		{
		case IK_HID_REQUEST_SET_PROTOCOL:
			ikdev_handle->Protocol = (uint8_t)(request->wValue);
            CONSOLE_PrintLn("[USB] SET_PROTOCOL: %i", ikdev_handle->Protocol);
			break;
		case IK_HID_REQUEST_GET_PROTOCOL:
            CONSOLE_PrintLn("[USB] SET_PROTOCOL: %i", ikdev_handle->Protocol);
			USBD_CtlSendData(device, (uint8_t*)&(ikdev_handle->Protocol), 1);
			break;
		case IK_HID_REQUEST_SET_IDLE:
			ikdev_handle->IdleState = (uint8_t)(request->wValue >> 8);
			break;
		case IK_HID_REQUEST_GET_IDLE:
			USBD_CtlSendData(device, (uint8_t*)&(ikdev_handle->IdleState), 1);
			break;
		case IK_HID_REQUEST_SET_REPORT:
			ikdev_handle->IsReportAvailable = 1;
			USBD_CtlPrepareRx(device, ikdev_handle->ReportBuffer, (uint8_t)(request->wLength));
			break;
		default:
			USBD_CtlError(device, request);
			return USBD_FAIL;
		}
		break;
	}

	return USBD_OK;
}

/*
 * Description
 * Retrieves the device's configuration descriptor.
 *
 * Parameters
 * - uint16_t* length: Data length returned.
 */
static uint8_t* IK_STM32CUBE_USBD_HID_GetConfigDescriptor(uint16_t* length)
{
	*length = sizeof(IK_USB_DESCRIPTOR_CONFIG);
	return IK_USB_DESCRIPTOR_CONFIG;
}

/*
 * Description
 * Retrieves the device's device qualifier descriptor.
 *
 * Parameters
 * - uint16_t* length: Data length returned.
 */
static uint8_t* IK_STM32CUBE_USBD_HID_GetDeviceQualifierDescriptor(uint16_t* length)
{
	*length = sizeof(_IK_USB_DESCRIPTOR_DEV_QUALIF);
	return _IK_USB_DESCRIPTOR_DEV_QUALIF;
}

/*
 * Description
 * Handler for EPIN data.
 *
 * Parameters
 * - USBD_HandleTypeDef* device: The device instance.
 * - uint8_t endpoint_num: The endpoint index.
 */
static uint8_t IK_STM32CUBE_USBD_HID_DataIn(USBD_HandleTypeDef* device, uint8_t endpoint_num)
{
	((IK_HID_DeviceHandle_t*) device->pClassData)->State = IK_HID_IDLE;
	return USBD_OK;
}

/*
 * Description
 * Handler for EPOUT data.
 *
 * Parameters
 * - USBD_HandleTypeDef* device: The device instance.
 * - uint8_t endpoint_num: The endpoint index.
 */
static uint8_t IK_STM32CUBE_USBD_HID_DataOut(USBD_HandleTypeDef* device, uint8_t endpoint_num)
{
	IK_HID_DeviceHandle_t* ikdev_handle = (IK_HID_DeviceHandle_t*) device->pClassData;

	((IK_HID_DeviceInterface_t*) device->pUserData)->OutEvent(ikdev_handle->ReportBuffer);
	USBD_LL_PrepareReceive(device, IK_HID_EPOUT_ADDR, ikdev_handle->ReportBuffer, IK_HID_OUT_REPORT_BUFFER_SIZE);

	return USBD_OK;
}

/*
 * Description
 * Handler for EP0 OUT data.
 *
 * Parameters
 * - USBD_HandleTypeDef* device: The device instance.
 */
static uint8_t IK_STM32CUBE_USBD_HID_EP0RecieveReady(USBD_HandleTypeDef* device)
{
	IK_HID_DeviceHandle_t* ikdev_handle = (IK_HID_DeviceHandle_t*) device->pClassData;

	if (ikdev_handle->IsReportAvailable == 1)
	{
		((IK_HID_DeviceInterface_t*) device->pUserData)->OutEvent(ikdev_handle->ReportBuffer);
		ikdev_handle->IsReportAvailable = 0;
	}

	return USBD_OK;
}

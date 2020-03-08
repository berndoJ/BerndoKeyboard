// infinikeys_usb.c
// Copyright (c) 2020 Johannes Berndorfer (berndoJ)

#include "infinikeys_usb.h"
#include "infinikeys_usb_desc.h"
#include "infinikeys_kbmatrix.h"
#include <stdlib.h>
#include <string.h>

static IK_USB_Init_t *_usb_init = NULL;

static uint8_t _usb_str_desc_buf[128];

static IK_USB_HID_Report_Protocol_t _usb_hid_protocol;

static uint8_t _IK_USB_CharToStrDescBuf(char *str);
static uint8_t _IK_USB_Strlen(char *str);

IK_Status_t IK_USB_Init(IK_USB_Init_t *init_struct)
{
    if (init_struct == NULL)
        return IK_ERROR;

    if (init_struct->USB_HW_Interface == NULL)
        return IK_ERROR;

    _usb_init = init_struct;

    // Update the descriptors to match the initialisation configuration.
    IK_USB_DESCRIPTOR_DEVICE[8] = IK_LOWBYTE(_usb_init->USB_VID); // Update VID
    IK_USB_DESCRIPTOR_DEVICE[9] = IK_HIGHBYTE(_usb_init->USB_VID);
    IK_USB_DESCRIPTOR_DEVICE[10] = IK_LOWBYTE(_usb_init->USB_PID); // Update PID
    IK_USB_DESCRIPTOR_DEVICE[11] = IK_LOWBYTE(_usb_init->USB_PID);
    IK_USB_DESCRIPTOR_LANGID[2] = IK_LOWBYTE(_usb_init->USB_LangID); // Update language ID string
    IK_USB_DESCRIPTOR_LANGID[3] = IK_HIGHBYTE(_usb_init->USB_LangID);

    // Set the current HID report protocol to BOOT protocol.
    IK_USB_SetHIDProtocol(IK_USB_HID_REPORT_PROTOCOL_BOOT);

    // Initialise the hardware interface.
    _usb_init->USB_HW_Interface->Init();

    return IK_OK;
}

IK_Status_t IK_USB_DeInit(void)
{
    // De-init the HW interface.
    if (_usb_init != NULL && _usb_init->USB_HW_Interface != NULL && _usb_init->USB_HW_Interface->DeInit != NULL)
        _usb_init->USB_HW_Interface->DeInit();

    // Reset the init struct pointer.
    _usb_init = NULL;

    return IK_OK;
}

IK_Status_t IK_USB_SetHIDProtocol(IK_USB_HID_Report_Protocol_t protocol)
{
    _usb_hid_protocol = protocol;
    return IK_OK;
}

IK_USB_HID_Report_Protocol_t IK_USB_GetHIDProtocol(void)
{
    return _usb_hid_protocol;
}

IK_Status_t IK_USB_SendHIDReport(uint8_t report_id, IK_Buffer_t report_buf)
{
    IK_Buffer_t xf_buf;
    IK_Status_t rstat;

    // Check for valid hardware interface.
    if (_usb_init == NULL || _usb_init->USB_HW_Interface == NULL ||
        _usb_init->USB_HW_Interface->TransmitUSB == NULL)
        return IK_ERROR;

    // Set up the transfer buffer.
    xf_buf.Size = report_buf.Size + 1;
    xf_buf.DataPtr = (uint8_t *) malloc(xf_buf.Size);

    // Check if the transfer buffer's memory has been allocated successfully.
    if (xf_buf.DataPtr == NULL)
        return IK_ERROR;

    // Fill the transfer buffer with data.
    xf_buf.DataPtr[0] = report_id;
    memcpy(xf_buf.DataPtr + 1, report_buf.DataPtr, report_buf.Size);

    // Transmit the transfer buffer via HW interface.
    rstat = _usb_init->USB_HW_Interface->TransmitUSB(xf_buf, IK_USB_HID_EP_NUM);

    // Free the buffer.
    free(xf_buf.DataPtr);

    return rstat;
}

IK_Buffer_t IK_USB_HW_GetUSBDescriptor(IK_USB_HW_Descriptors_t desc_type)
{
    IK_Buffer_t buf;
    uint8_t sdesc_size;

    switch (desc_type)
    {
        case IK_USB_HW_DEVICE_DESCRIPTOR:
            buf.Size = IK_USB_DESCRIPTOR_DEVICE_SIZE;
            buf.DataPtr = IK_USB_DESCRIPTOR_DEVICE;
            break;
        case IK_USB_HW_CONFIGURATION_DESCRIPTOR:
            buf.Size = IK_USB_DESCRIPTOR_CONFIG_SIZE;
            buf.DataPtr = IK_USB_DESCRIPTOR_CONFIG;
            break;
        case IK_USB_HW_HID_REPORT_DESCRIPTOR:
            buf.Size = IK_USB_DESCRIPTOR_HID_REPORT_SIZE;
            buf.DataPtr = IK_USB_DESCRIPTOR_HID_REPORT;
            break;
        case IK_USB_HW_LANGID_DESCRIPTOR:
            buf.Size = IK_USB_DESCRIPTOR_LANGID_SIZE;
            buf.DataPtr = IK_USB_DESCRIPTOR_LANGID;
            break;
        case IK_USB_HW_MFC_STRING_DESCRIPTOR:
            sdesc_size = _IK_USB_CharToStrDescBuf((char *) _usb_init->Manufacturer_Name);
            buf.Size = sdesc_size;
            buf.DataPtr = _usb_str_desc_buf;
            break;
        case IK_USB_HW_PRODUCT_STRING_DESCRIPTOR:
            sdesc_size = _IK_USB_CharToStrDescBuf((char *) _usb_init->Product_Name);
            buf.Size = sdesc_size;
            buf.DataPtr = _usb_str_desc_buf;
            break;
        case IK_USB_HW_SERIAL_NUMBER_STRING_DESCRIPTOR:
            sdesc_size = _IK_USB_CharToStrDescBuf((char *) _usb_init->Serial_Number_String);
            buf.Size = sdesc_size;
            buf.DataPtr = _usb_str_desc_buf;
            break;
        default:
            buf.Size = 0;
            buf.DataPtr = NULL;
            break;
    }

    return buf;
}

IK_Status_t IK_USB_HW_ReceiveUSB(IK_Buffer_t data, uint8_t ep_addr)
{
    // TODO: Implement the handling of recieved data.
    switch (ep_addr)
    {
        case 1:
            // HID report for indicator LEDs.
            break;
    }
    return IK_OK;
}

void IK_USB_HW_SOFCallback(void)
{
    // Invoke the send HID report function.
    IK_KBM_SendHIDReport();
}

static uint8_t _IK_USB_CharToStrDescBuf(char *str)
{
    uint8_t i = 0, len;

    if (str == NULL)
        return 0;

    len = (_IK_USB_Strlen(str) * 2) + 2;
    _usb_str_desc_buf[i++] = len;
    _usb_str_desc_buf[i++] = IK_USB_DESCRIPTOR_TYPE_STRING;

    while (*str != '\0')
    {
        _usb_str_desc_buf[i++] = *(str++);
        _usb_str_desc_buf[i++] = 0x00;
    }

    return len;
}

static uint8_t _IK_USB_Strlen(char *str)
{
    uint8_t len = 0;

    while (*str != '\0')
    {
        len++;
        str++;
    }

    return len;
}
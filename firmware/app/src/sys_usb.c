#include "sys_usb.h"
#include "error.h"
#include "console.h"
#include "stm32f1xx_hal.h"
#include "infinikeys.h"
#include "infinikeys_stm32cube_usb_hid.h"
#include "infinikeys_stm32cube_usb_if.h"
#include <stdint.h>

USBD_HandleTypeDef *hw_usbd_handle = NULL;

uint8_t _IK_USB_DESCRIPTOR_DEV_QUALIF[_IK_USB_DESCRIPTOR_DEV_QUALIF_SIZE] =
{
    _IK_USB_DESCRIPTOR_DEV_QUALIF_SIZE,             // bLength => 18 bytes
    IK_USB_DESCRIPTOR_TYPE_DEVICE_QUALIFIER,        // bDescriptorType => Device descriptor
    0x00, 0x02,                                     // bcdUSB => USB 2.0
    0x00,                                           // bDeviceClass
    0x00,                                           // bDeviceSubClass
    0x00,                                           // bDeviceProtocol
    IK_USB_EP0_MAX_PACKET_SIZE,                     // bMaxPacketSize0 (EP0 MaxPacketSize)
    1U,                                             // bNumConfigurations => 1 Config
    0x00                                            // bReserved => Set to 0x00
};

static uint8_t *GetDeviceDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
static uint8_t *GetLangIDStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
static uint8_t *GetManufacturerStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
static uint8_t *GetProductStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
static uint8_t *GetSerialStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
static uint8_t *GetConfigurationStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
static uint8_t *GetInterfaceStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);

static USBD_DescriptorsTypeDef _usbd_desc =
{
    .GetDeviceDescriptor = GetDeviceDescriptor,
    .GetLangIDStrDescriptor = GetLangIDStrDescriptor,
    .GetManufacturerStrDescriptor = GetManufacturerStrDescriptor,
    .GetProductStrDescriptor = GetProductStrDescriptor,
    .GetSerialStrDescriptor = GetSerialStrDescriptor,
    .GetConfigurationStrDescriptor = GetConfigurationStrDescriptor,
    .GetInterfaceStrDescriptor = GetInterfaceStrDescriptor
};

void SYS_USB_Init(void)
{
    if (hw_usbd_handle != NULL)
    {
        free(hw_usbd_handle);
        hw_usbd_handle = NULL;
    }

    hw_usbd_handle = (USBD_HandleTypeDef *) malloc(sizeof(USBD_HandleTypeDef));

    if (hw_usbd_handle == NULL)
        SYS_ThrowError(SYSERR_FATAL_ERROR, __LINE__, __FILE__, "USBD-handle allocation failed.");

    CONSOLE_PrintLn("[SYS-USB] Allocated USBD-handle memory.");

    if (USBD_Init(hw_usbd_handle, &_usbd_desc, DEVICE_FS) != USBD_OK)
        SYS_ThrowError(SYSERR_FATAL_ERROR, __LINE__, __FILE__, "USBD-init failed.");

    CONSOLE_PrintLn("[SYS-USB] Initialised USBD handle.");

    if (USBD_RegisterClass(hw_usbd_handle, &IK_HID_Class) != USBD_OK)
        SYS_ThrowError(SYSERR_FATAL_ERROR, __LINE__, __FILE__, "USBD class registration failed.");
    
    CONSOLE_PrintLn("[SYS-USB] Registered InfiniKeys USBD class.");

    if (IK_STM32CUBE_USBD_HID_RegisterInterface(hw_usbd_handle, &IK_USBD_HID_Itf) != USBD_OK)
        SYS_ThrowError(SYSERR_FATAL_ERROR, __LINE__, __FILE__, "USBD interface registration failed.");
    
    CONSOLE_PrintLn("[SYS-USB] Registered InfiniKeys USBD interface.");

    if (USBD_Start(hw_usbd_handle) != USBD_OK)
        SYS_ThrowError(SYSERR_FATAL_ERROR, __LINE__, __FILE__, "USBD port-start failed.");
    
    CONSOLE_PrintLn("[SYS-USB] Initialised USBD. Started USB connection.");
}

void SYS_USB_DeInit(void)
{
    free(hw_usbd_handle);
    hw_usbd_handle = NULL;
}

static uint8_t *GetDeviceDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
    (void) speed;

    IK_Buffer_t desc = IK_USB_HW_GetUSBDescriptor(IK_USB_HW_DEVICE_DESCRIPTOR);

    *length = (uint16_t) desc.Size;
    return desc.DataPtr;    
}

static uint8_t *GetLangIDStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
    (void) speed;

    IK_Buffer_t desc = IK_USB_HW_GetUSBDescriptor(IK_USB_HW_LANGID_DESCRIPTOR);

    *length = (uint16_t) desc.Size;
    return desc.DataPtr;
}

static uint8_t *GetManufacturerStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
    (void) speed;

    IK_Buffer_t desc = IK_USB_HW_GetUSBDescriptor(IK_USB_HW_MFC_STRING_DESCRIPTOR);

    *length = (uint16_t) desc.Size;
    return desc.DataPtr;
}

static uint8_t *GetProductStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
    (void) speed;

    IK_Buffer_t desc = IK_USB_HW_GetUSBDescriptor(IK_USB_HW_PRODUCT_STRING_DESCRIPTOR);

    *length = (uint16_t) desc.Size;
    return desc.DataPtr;
}

static uint8_t *GetSerialStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
    (void) speed;

    IK_Buffer_t desc = IK_USB_HW_GetUSBDescriptor(IK_USB_HW_SERIAL_NUMBER_STRING_DESCRIPTOR);

    *length = (uint16_t) desc.Size;
    return desc.DataPtr;
}

static uint8_t *GetConfigurationStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
    (void) speed;

    IK_Buffer_t desc = IK_USB_HW_GetUSBDescriptor(IK_USB_HW_SERIAL_NUMBER_STRING_DESCRIPTOR);

    *length = (uint16_t) desc.Size;
    return desc.DataPtr;
}

static uint8_t *GetInterfaceStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
    (void) speed;

    IK_Buffer_t desc = IK_USB_HW_GetUSBDescriptor(IK_USB_HW_SERIAL_NUMBER_STRING_DESCRIPTOR);

    *length = (uint16_t) desc.Size;
    return desc.DataPtr;
}

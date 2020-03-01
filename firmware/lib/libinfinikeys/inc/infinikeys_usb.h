/*******************************************************************************
* \file infinikeys_usb.h
* \date 14.01.2020
********************************************************************************
* \author Johannes Berndorfer (berndoJ)
* \copyright Copyright (c) 2020 Johannes Berndorfer (berndoJ)
********************************************************************************
* \brief Module for all USB-related tasks and configuration regarding the
*        InfiniKeys library.
*******************************************************************************/

#if !defined(__INFINIKEYS_USB_H)
#define __INFINIKEYS_USB_H

#include "infinikeys_def.h"
#include "infinikeys_hwif.h"

#define IK_USB_EP0_MAX_PACKET_SIZE                  64U // Must be 64 for USB-HS
#define IK_USB_HID_COUNTRY_CODE                     0x00 // Country code "reserved"
#define IK_USB_DEFAULT_VID                          1155U
#define IK_USB_DEFAULT_PID                          1024U

#define IK_USB_HID_EPIN_SIZE                        0x08
#define IK_USB_HID_EPIN_INTERVAL                    5U
#define IK_USB_HID_EPOUT_SIZE                       0x08
#define IK_USB_HID_EPOUT_INTERVAL                   5U
#define IK_USB_HID_EP_NUM                           1U
#define IK_USB_HID_KEY_ROLLOVER                     8U
#define IK_USB_HID_REPORT_ID_KEYBOARD               0x01
#define IK_USB_HID_REPORT_ID_DATA                   0x02

#define IK_USB_STR_DESC_LANGID                      0x00
#define IK_USB_STR_DESC_MFC                         0x01
#define IK_USB_STR_DESC_PRODUCT                     0x02
#define IK_USB_STR_DESC_SERIAL                      0x03

#define IK_USB_DESCRIPTOR_TYPE_DEVICE               0x01
#define IK_USB_DESCRIPTOR_TYPE_CONFIGURATION        0x02
#define IK_USB_DESCRIPTOR_TYPE_STRING               0x03
#define IK_USB_DESCRIPTOR_TYPE_INTERFACE            0x04
#define IK_USB_DESCRIPTOR_TYPE_ENDPOINT             0x05
#define IK_USB_DESCRIPTOR_TYPE_DEVICE_QUALIFIER     0x06
#define IK_USB_DESCRIPTOR_TYPE_OTHER_SPEED_CONFIG   0x07
#define IK_USB_DESCRIPTOR_TYPE_BOS                  0x08
#define IK_USB_DESCRIPTOR_TYPE_HID                  0x21
#define IK_USB_DESCRIPTOR_TYPE_HID_REPORT           0x22

/**
 * \brief Specified a type of USB descriptor used by InfiniKeys-USB.
 */
typedef enum _IK_USB_HW_Descriptors
{
    IK_USB_HW_DEVICE_DESCRIPTOR,
    IK_USB_HW_CONFIGURATION_DESCRIPTOR,
    IK_USB_HW_HID_REPORT_DESCRIPTOR,
    IK_USB_HW_LANGID_DESCRIPTOR,
    IK_USB_HW_MFC_STRING_DESCRIPTOR,
    IK_USB_HW_PRODUCT_STRING_DESCRIPTOR,
    IK_USB_HW_SERIAL_NUMBER_STRING_DESCRIPTOR
} IK_USB_HW_Descriptors_t;

/**
 * \brief Initialisation structure for providing initialisation info.
 */
typedef struct _IK_USB_Init
{
    IK_USB_HW_Interface_t *USB_HW_Interface;
    uint16_t USB_VID;
    uint16_t USB_PID;
    uint16_t USB_LangID;
    const char *Manufacturer_Name;
    const char *Product_Name;
    const char *Serial_Number_String;
} IK_USB_Init_t;

/**
 * \brief Initialisation function for the InfiniKeys-USB module.
 * \param init_struct The initialisation configuration.
 * \return The return status of the function.
 */
IK_Status_t IK_USB_Init(IK_USB_Init_t *init_struct);

/**
 * \brief De-initialisation function for the InfiniKeys-USB module.
 * \return The return status of the function.
 */
IK_Status_t IK_USB_DeInit(void);

/**
 * \brief Transmits the given report (buffer) to the USB host as a HID-report
 *        with the given report id.
 * \param report_id The report id of the report to send.
 * \param report_buf The buffer containing the contents of the report.
 * \return The return status of the function.
 */
IK_Status_t IK_USB_SendHIDReport(uint8_t report_id, IK_Buffer_t report_buf);

/**
 * \brief Interface for the USB hardware implementation to get the needed USB
 *        descriptors for setting up the hardware side of the USB
 *        implementation.
 * \param desc_type The type of descriptor to get.
 * \return The descriptor buffer.
 */
IK_Buffer_t IK_USB_HW_GetUSBDescriptor(IK_USB_HW_Descriptors_t desc_type);

/**
 * \brief Callback function for the USB hardware interface for handling recieved
 *        USB data and delegating it back to InifniKeys-USB.
 * \param data The recieved data in a buffer.
 * \param ep_addr The address of the endpoint which recieved the data.
 * \return The return status of the function.
 */
IK_Status_t IK_USB_HW_ReceiveUSB(IK_Buffer_t data, uint8_t ep_addr);

/**
 * \brief Callback function for the USB hardware interface for handling start of
 *        frame events on the USB bus. This callback is required because a
 *        keyboard report is sent every frame. Thus, a SOF event invokes the
 *        transmission of a keyboard state report.
 */
void IK_USB_HW_SOFCallback(void);

#endif // __INFINIKEYS_USB_H
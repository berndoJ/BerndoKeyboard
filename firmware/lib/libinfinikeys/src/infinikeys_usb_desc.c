// infinikeys_usb_desc.c
// Copyright (c) 2020 Johannes Berndorfer (berndoJ)

#include "infinikeys_usb_desc.h"

uint8_t IK_USB_DESCRIPTOR_DEVICE[IK_USB_DESCRIPTOR_DEVICE_SIZE] __attribute__((aligned(4))) =
{
    IK_USB_DESCRIPTOR_DEVICE_SIZE,                  // bLength => 18 bytes
    IK_USB_DESCRIPTOR_TYPE_DEVICE,                  // bDescriptorType => Device descriptor
    0x00, 0x02,                                     // bcdUSB => USB 2.0
    0x00,                                           // bDeviceClass
    0x00,                                           // bDeviceSubClass
    0x00,                                           // bDeviceProtocol
    IK_USB_EP0_MAX_PACKET_SIZE,                     // bMaxPacketSize0 (EP0 MaxPacketSize)
    IK_LOWBYTE(IK_USB_DEFAULT_VID),                 // idVendor:0
    IK_HIGHBYTE(IK_USB_DEFAULT_VID),                // idVendor:1
    IK_LOWBYTE(IK_USB_DEFAULT_PID),                 // idProduct:0
    IK_HIGHBYTE(IK_USB_DEFAULT_PID),                // idProduct:1
    0x00, 0x02,                                     // bcdDevice => USB 2.0
    IK_USB_STR_DESC_MFC,                            // iManufacturerString
    IK_USB_STR_DESC_PRODUCT,                        // iProductString
    IK_USB_STR_DESC_SERIAL,                         // iSerialNumberString
    1U                                              // bNumConfigurations => 1 Config
};

uint8_t IK_USB_DESCRIPTOR_CONFIG[IK_USB_DESCRIPTOR_CONFIG_SIZE] __attribute__((aligned(4))) =
{
    // -> Configuration Descriptor <-               //
    9U,                                             // bLength => 9 bytes
    IK_USB_DESCRIPTOR_TYPE_CONFIGURATION,           // bDescriptorType => Configuration descriptor
    IK_LOWBYTE(IK_USB_DESCRIPTOR_CONFIG_SIZE),      // wTotalLength:0 => Size of this array
    IK_HIGHBYTE(IK_USB_DESCRIPTOR_CONFIG_SIZE),     // wTotalLength:1 => Size of this array
    1U,                                             // bNumInterfaces => 1 Interface
    0x01,                                           // bConfigurationValue => Config ID 1
    0x00,                                           // iConfiguration => No string
    0b10000000,                                     // bmAttributes => Not self-powered, remote wakeup disabled.
    250U,                                           // bMaxPower => 500mA
    // -> Interface 0: HID <-                       //
    9U,                                             // bLength => 9 bytes
    IK_USB_DESCRIPTOR_TYPE_INTERFACE,               // bDescriptorType => Interface descriptor
    0x00,                                           // bInterfaceNumber => Interface ID 0
    0x00,                                           // bAlternateSetting => Alternate ID 0
    2U,                                             // bNumEndpoints => 2 EPs.
    0x03,                                           // bInterfaceClass => HID-Class
    0x00,                                           // bInterfaceSubClass => Subclass 00h
    0x00,                                           // bInterfaceProtocol => Protocol 00h
    0x00,                                           // iInterface => No string
    // -> Interface 0: HID Descriptor <-            //
    9U,                                             // bLength => 9 bytes
    IK_USB_DESCRIPTOR_TYPE_HID,                     // bDescriptorType => HID descriptor.
    0x11, 0x01,                                     // bcdHID => HID 1.11
    IK_USB_HID_COUNTRY_CODE,                        // bCountryCode
    1U,                                             // bNumDescriptors => 1 Descriptor
    IK_USB_DESCRIPTOR_TYPE_HID_REPORT,              // bDescriptorType => HID Report Descriptor
    IK_LOWBYTE(IK_USB_DESCRIPTOR_HID_REPORT_SIZE),  // wDescriptorLength:0
    IK_HIGHBYTE(IK_USB_DESCRIPTOR_HID_REPORT_SIZE), // wDescriptorLength:1
    // -> Interface 0: Endpoint EPIN <-             //
    7U,                                             // bLength => 7 bytes
    IK_USB_DESCRIPTOR_TYPE_ENDPOINT,                // bDescriptorType => Endpoint Descriptor
    0b10000000 | IK_USB_HID_EP_NUM,                 // bEndpointAddress => EP1 -> Direction In
    0b00000011,                                     // bmAttributes => Interrupt, No Sync, Data EP
    IK_LOWBYTE(IK_USB_HID_EPIN_SIZE),               // wMaxPacketSize:0
    IK_HIGHBYTE(IK_USB_HID_EPIN_SIZE),              // wMaxPacketSize:1
    IK_USB_HID_EPIN_INTERVAL,                       // bInterval
    // -> Interface 0: Endpoint EPOUT <-            //
    7U,                                             // bLength => 7 bytes
    IK_USB_DESCRIPTOR_TYPE_ENDPOINT,                // bDescriptorType => Endpoint Descriptor
    0b00000000 | IK_USB_HID_EP_NUM,                 // bEndpointAddress => EP1 -> Direction Out
    0b00000011,                                     // bmAttributes => Interrupt, No Sync, Data EP
    IK_LOWBYTE(IK_USB_HID_EPOUT_SIZE),              // wMaxPacketSize:0
    IK_HIGHBYTE(IK_USB_HID_EPOUT_SIZE),             // wMaxPacketSize:1
    IK_USB_HID_EPOUT_INTERVAL                       // bInterval
};

uint8_t IK_USB_DESCRIPTOR_HID_REPORT[IK_USB_DESCRIPTOR_HID_REPORT_SIZE] __attribute__((aligned(4))) =
{
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x06,                    // USAGE (Keyboard)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x85, 0x01,                    //   REPORT_ID (1)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x08,                    //   REPORT_COUNT (8)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)
    0x95, 0x05,                    //   REPORT_COUNT (5)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x05, 0x08,                    //   USAGE_PAGE (LEDs)
    0x19, 0x01,                    //   USAGE_MINIMUM (Num Lock)
    0x29, 0x05,                    //   USAGE_MAXIMUM (Kana)
    0x91, 0x02,                    //   OUTPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x03,                    //   REPORT_SIZE (3)
    0x91, 0x03,                    //   OUTPUT (Cnst,Var,Abs)
    0x95, 0x06,                    //   REPORT_COUNT (6)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x65,                    //   LOGICAL_MAXIMUM (101)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)
    0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
    0xc0                           // END_COLLECTION
    /*
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x06,                    // USAGE (Keyboard)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x85, 0x01,                    //   REPORT_ID (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x08,                    //   REPORT_COUNT (8)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x05,                    //   REPORT_COUNT (5)
    0x05, 0x08,                    //   USAGE_PAGE (LEDs)
    0x19, 0x01,                    //   USAGE_MINIMUM (Num Lock)
    0x29, 0x05,                    //   USAGE_MAXIMUM (Kana)
    0x91, 0x02,                    //   OUTPUT (Data,Var,Abs)
    0x75, 0x03,                    //   REPORT_SIZE (3)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x91, 0x03,                    //   OUTPUT (Cnst,Var,Abs)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x06,                    //   REPORT_COUNT (6)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0x01,                    //   USAGE_MINIMUM (Keyboard ErrorRollOver)
    0x29, 0xa4,                    //   USAGE_MAXIMUM (Keyboard ExSel)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x85, 0x02,                    //   REPORT_ID (2)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x08,                    //   REPORT_COUNT (8)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x05,                    //   REPORT_COUNT (5)
    0x05, 0x08,                    //   USAGE_PAGE (LEDs)
    0x19, 0x01,                    //   USAGE_MINIMUM (Num Lock)
    0x29, 0x05,                    //   USAGE_MAXIMUM (Kana)
    0x91, 0x02,                    //   OUTPUT (Data,Var,Abs)
    0x75, 0x03,                    //   REPORT_SIZE (3)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x91, 0x03,                    //   OUTPUT (Cnst,Var,Abs)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x96, 0x00, 0x01,              //   REPORT_COUNT (256)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x19,                    //   REPORT_COUNT (25)
    0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)
    0xc0,                          // END_COLLECTION
    0x06, 0x00, 0xff,              // USAGE_PAGE (Vendor Defined Page 1)
    0x09, 0x01,                    // USAGE (Vendor Usage 1)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x85, 0x03,                    //   REPORT_ID (3)
    0x06, 0x00, 0xff,              //   USAGE_PAGE (Vendor Defined Page 1)
    0x09, 0x01,                    //   USAGE (Vendor Usage 1)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x40,                    //   REPORT_COUNT (64)
    0x91, 0x02,                    //   OUTPUT (Data,Var,Abs)
    0xc0                           // END_COLLECTION
    
    
    
    
    
    
    
    
    
    
    0x05, 0x01,                                     // Usage Page: Generic Desktop
    0x09, 0x06,                                     // Usage: Keyboard
    0xa1, 0x01,                                     // Collection: Application
                                                    //
                                                    //     --- Standard boot-protocol HID keyboard report ---
    0x85, 0x01,                                     //     Report ID: 1
                                                    //     -> [IN] Modifier Key Byte <-
    0x75, 0x01,                                     //     Report Size: 1
    0x95, 0x08,                                     //     Report Count: 8
    0x05, 0x07,                                     //     Usage Page: Key Codes (Keyboard)
    0x19, 0xe0,                                     //     Usage Minimum: E0h -> Left-Control
    0x29, 0xe7,                                     //     Usage Maximum: E7h -> Right-GUI
    0x15, 0x00,                                     //     Logical Minimum: 00h
    0x25, 0x01,                                     //     Logical Maximum: 01h
    0x81, 0x02,                                     //     Input: Data, Variable, Absolute
                                                    //     -> [IN] Reserved Byte <-
    0x75, 0x08,                                     //     Report Size: 8
    0x95, 0x01,                                     //     Report Count: 1
    0x81, 0x03,                                     //     Input: Constant, Variable, Absolute
                                                    //     -> [OUT] LED State Report <-
    0x75, 0x01,                                     //     Report Size: 1
    0x95, 0x05,                                     //     Report Count: 5
    0x05, 0x08,                                     //     Usage Page: LEDs
    0x19, 0x01,                                     //     Usage Minimum: 01h -> Num-Lock
    0x29, 0x05,                                     //     Usage Maximum: 05h -> Kana
    0x91, 0x02,                                     //     Output: Data, Variable, Absolute
                                                    //     -> [OUT] LED State Report Padding <-
    0x75, 0x03,                                     //     Report Size: 3
    0x95, 0x01,                                     //     Report Count: 1
    0x91, 0x03,                                     //     Output: Constant, Variable, Absolute
                                                    //     -> [IN] Key State Report <-
    0x75, 0x08,                                     //     Report Size: 8
    0x95, 0x06,                                     //     Report Count: 6
    0x05, 0x07,                                     //     Usage Page: Key Codes (Keyboard)
    0x19, 0x00,                                     //     Usage Minimum: 00h
    0x29, 0xa4,                                     //     Usage Maximum: FFh
    0x15, 0x00,                                     //     Logical Minimum: 00h
    0x26, 0xff, 0x00,                               //     Logical Maximum: FFh
    0x81, 0x02,                                     //     Input: Data, Variable, Absolute
                                                    //
                                                    //     --- NKRO non-boot-protocol HID keyboard report ---
    0x85, 0x02,                                     //     Report ID: 2
                                                    //     -> [IN] Modifier Key Byte <-
    0x75, 0x01,                                     //     Report Size: 1
    0x95, 0x08,                                     //     Report Count: 8
    0x05, 0x07,                                     //     Usage Page: Key Codes (Keyboard)
    0x19, 0xe0,                                     //     Usage Minimum: E0h -> Left-Control
    0x29, 0xe7,                                     //     Usage Maximum: E7h -> Right-GUI
    0x15, 0x00,                                     //     Logical Minimum: 0
    0x25, 0x01,                                     //     Logical Maximum: 1
    0x81, 0x02,                                     //     Input: Data, Variable, Absolute
                                                    //     -> [OUT] LED State Report <-
    0x75, 0x01,                                     //     Report Size: 1
    0x95, 0x05,                                     //     Report Count: 5
    0x05, 0x08,                                     //     Usage Page: LEDs
    0x19, 0x01,                                     //     Usage Minimum: 01h -> Num Lock
    0x29, 0x05,                                     //     Usage Maximum: 05h -> Kana
    0x91, 0x02,                                     //     Output: Data, Variable, Absolute
                                                    //     -> [OUT] LED State Report Padding <-
    0x75, 0x03,                                     //     Report Size: 3
    0x95, 0x01,                                     //     Report Count: 1
    0x91, 0x03,                                     //     Output: Constant, Variable, Absolute
                                                    //     -> [IN] Key State Bitmap Report <-
    0x75, 0x01,                                     //     Report Size: 1
    0x96, 0x00, 0x01,                               //     Report Count: 256
    0x05, 0x07,                                     //     Usage Page: Key Codes (Keyboard)
    0x19, 0x00,                                     //     Usage Minimum: 00h
    0x29, 0xe7,                                     //     Usage Maximum: FFh
    0x15, 0x00,                                     //     Logical Minimum: 0
    0x25, 0x01,                                     //     Logical Maximum: 1
    0x81, 0x02,                                     //     Input: Data, Variable, Absolute
                                                    //
    0xc0,                                           // End Collection
                                                    //
    0x06, 0x00, 0xff,                               // Usage Page: Vendor Defined Page
    0x09, 0x01,                                     // Usage: Vendor Defined Usage 1
    0xa1, 0x01,                                     // Collection: Application
                                                    //
                                                    //     --- Configuration Command Report ---
    0x85, 0x03,                                     //     Report ID: 3
    0x06, 0x00, 0xff,                               //     Usage Page: Vendor Defined Page
    0x09, 0x01,                                     //     Usage: Vendor Defined Usage 1
    0x15, 0x00,                                     //     Logical Minimum: 0
    0x26, 0xff, 0x00,                               //     Logical Maximum: 255
    0x75, 0x08,                                     //     Report Size: 8
    0x95, 0x40,                                     //     Report Count: 64
    0x91, 0x02,                                     //     Output: Data, Variable, Absolute
                                                    //
    0xc0                                            // End Collection */
};

uint8_t IK_USB_DESCRIPTOR_LANGID[IK_USB_DESCRIPTOR_LANGID_SIZE] __attribute__((aligned(4))) =
{
    IK_USB_DESCRIPTOR_LANGID_SIZE,                  // bLength => 4 bytes
    IK_USB_DESCRIPTOR_TYPE_STRING,                  // bDescriptorType => String descriptor
    0x00,                                           // wLangID:0 => Not Set (00h)
    0x00                                            // wLangID:1 => Not Set (00h)
};


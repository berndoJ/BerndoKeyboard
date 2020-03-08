/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_custom_hid_if.c
  * @version        : v2.0_Cube
  * @brief          : USB Device Custom HID interface file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "usbd_custom_hid_if.h"

/* USER CODE BEGIN INCLUDE */

/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device.
  * @{
  */

/** @addtogroup USBD_CUSTOM_HID
  * @{
  */

/** @defgroup USBD_CUSTOM_HID_Private_TypesDefinitions USBD_CUSTOM_HID_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */

/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Defines USBD_CUSTOM_HID_Private_Defines
  * @brief Private defines.
  * @{
  */

/* USER CODE BEGIN PRIVATE_DEFINES */

/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Macros USBD_CUSTOM_HID_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Variables USBD_CUSTOM_HID_Private_Variables
  * @brief Private variables.
  * @{
  */

/** Usb HID report descriptor. */
__ALIGN_BEGIN static uint8_t CUSTOM_HID_ReportDesc_FS[USBD_CUSTOM_HID_REPORT_DESC_SIZE] __ALIGN_END =
{
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
    0x29, 0xff,                                     //     Usage Maximum: FFh
    0x15, 0x00,                                     //     Logical Minimum: 00h
    0x25, 0xff,                                     //     Logical Maximum: FFh
    0x81, 0x00,                                     //     Input: Data, Variable, Absolute
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
    0x95, 0xff,                                     //     Report Count: 256
    0x05, 0x07,                                     //     Usage Page: Key Codes (Keyboard)
    0x19, 0x00,                                     //     Usage Minimum: 00h
    0x29, 0xff,                                     //     Usage Maximum: FFh
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
    0x25, 0xff,                                     //     Logical Maximum: 255
    0x75, 0x08,                                     //     Report Size: 8
    0x95, 0x40,                                     //     Report Count: 64
    0x91, 0x02,                                     //     Output: Data, Variable, Absolute
                                                    //
    0xc0                                            // End Collection
};

/* USER CODE BEGIN PRIVATE_VARIABLES */

/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Exported_Variables USBD_CUSTOM_HID_Exported_Variables
  * @brief Public variables.
  * @{
  */
extern USBD_HandleTypeDef hUsbDeviceFS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */
/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_FunctionPrototypes USBD_CUSTOM_HID_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static int8_t CUSTOM_HID_Init_FS(void);
static int8_t CUSTOM_HID_DeInit_FS(void);
static int8_t CUSTOM_HID_OutEvent_FS(uint8_t event_idx, uint8_t state);

/**
  * @}
  */

USBD_CUSTOM_HID_ItfTypeDef USBD_CustomHID_fops_FS =
{
  CUSTOM_HID_ReportDesc_FS,
  CUSTOM_HID_Init_FS,
  CUSTOM_HID_DeInit_FS,
  CUSTOM_HID_OutEvent_FS
};

/** @defgroup USBD_CUSTOM_HID_Private_Functions USBD_CUSTOM_HID_Private_Functions
  * @brief Private functions.
  * @{
  */

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes the CUSTOM HID media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_Init_FS(void)
{
  /* USER CODE BEGIN 4 */
  return (USBD_OK);
  /* USER CODE END 4 */
}

/**
  * @brief  DeInitializes the CUSTOM HID media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_DeInit_FS(void)
{
  /* USER CODE BEGIN 5 */
  return (USBD_OK);
  /* USER CODE END 5 */
}

/**
  * @brief  Manage the CUSTOM HID class events
  * @param  event_idx: Event index
  * @param  state: Event state
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_OutEvent_FS(uint8_t event_idx, uint8_t state)
{
  /* USER CODE BEGIN 6 */
  return (USBD_OK);
  /* USER CODE END 6 */
}

/* USER CODE BEGIN 7 */
/**
  * @brief  Send the report to the Host
  * @param  report: The report to be sent
  * @param  len: The report length
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
/*
static int8_t USBD_CUSTOM_HID_SendReport_FS(uint8_t *report, uint16_t len)
{
  return USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, report, len);
}
*/
/* USER CODE END 7 */

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */

/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


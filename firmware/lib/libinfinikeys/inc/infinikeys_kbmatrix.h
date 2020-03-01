/*******************************************************************************
* \file infinikeys_kbmatrix.h
* \date 29.01.2020
********************************************************************************
* \author Johannes Berndorfer (berndoJ)
* \copyright Copyright (c) 2020 Johannes Berndorfer (berndoJ)
********************************************************************************
* \brief Keyboard matrix mapping and switching handler module.
*
* \note The following note describes the structure of the keyboard-matrix config
*       item:
*       + [0] KeyLayer ID : 0x00 to 0xff
*       + [1..2] KeyLayer Size (count of KeyDescriptors) : 0x0000 to 0xffff
*       + [3..n] KeyDescriptors:
*         - [k+0..1] KeyCode : 0x0001 to 0xffff
*         - [k+2] KeyType : See enum definition for more info.
*         - [k+3..6] KeyData : HID-Code, key-layer, action-id (type-specific)
*       + [b] KeyLayer ID of another layer
*       + As from 0 to b-1.
*******************************************************************************/

#if !defined(__INFINIKEYS_KBMATRIX_H)
#define __INFINIKEYS_KBMATRIX_H

#include "infinikeys_def.h"

#define IK_KBM_KEYBOARD_MATRIX_CFG_KEY          0xff000001

/**
 * \brief Defines the type of a keystroke -> If the keystroke means that the key
 *        was pressed or released.
 */
typedef enum _IK_KeystrokeType
{
    IK_KEYSTROKE_TYPE_PRESS, /**< The key the keystroke refers to is pressed. */
    IK_KEYSTROKE_TYPE_RELEASE /**< The key the keystroke refers to is
                                   released. */
} IK_KeystrokeType_t;

/**
 * \brief Defines the type of a key. The type of a key determines its function.
 */
typedef enum _IK_KeyType
{
    IK_KEYTYPE_NONE = 0U, /**< The key is a dummy - no action. */
    IK_KEYTYPE_HID = 1U, /**< The key is a HID key. The HID-code is stored in
                              KeyData. */
    IK_KEYTYPE_HID_MODIFIER = 2U, /**< The key is a HID-modifier key. The HID-
                                       modifier type is stored in KeyData. */
    IK_KEYTYPE_KEY_LAYER_SWITCH = 3U, /**< The key is a key-layer-switch. The
                                           key-layer this switches to is stored
                                           in KeyData. */
    IK_KEYTYPE_ACTION = 4U /**< The key is an action-key. This key activates an
                                action on pressing it. The action id is stored
                                in KeyData. */
} IK_KeyType_t;

/**
 * \brief Describes a keystroke. A keystroke is either the change of state of a
 *        key from not-pressed to pressed or vice-versa.
 */
typedef struct _IK_Keystroke
{
    uint16_t KeyCode; /**< The key code of the key the keystroke refers to. */

    IK_KeystrokeType_t Type; /**< The type of keystroke this refers to. */
} IK_Keystroke_t;

typedef struct _IK_Key
{
    uint16_t KeyCode;
    IK_KeyType_t KeyType;
    uint32_t KeyData;
} IK_Key_t;
#define IK_KEY_NULL { 0, 0, 0 }

/**
 * \brief Initialisation configuration data struct.
 */
typedef struct _IK_KBM_Init
{
    void *Placeholder;
} IK_KBM_Init_t;

/**
 * \brief Initialises this module.
 * \return The return status of this function.
 */
IK_Status_t IK_KBM_Init();

/**
 * \brief De-initialises this module.
 * \return The return status of this function.
 */
IK_Status_t IK_KBM_DeInit(void);

/**
 * \brief Retrieves the information about a key with the given key code on the
 *        given key layer.
 * \param key_code The key code of the key to retrieve information about.
 * \param key_layer The keyboard key layer on which the key is placed.
 * \return The key.
 */
IK_Key_t IK_KBM_RetrieveKeyInfo(uint16_t key_code, uint8_t key_layer);

/**
 * \brief Report function for reporting InfiniKeys-KeyboardMatrix that a key has
 *        either been pressed or released on the keyboard switch matrix. This
 *        function looks at the key pressed/released and decides what action to
 *        perform for handling the keystroke. This action could refer to sending
 *        a new HID-report to the USB host or switch the key-layer.
 * \param keystroke The keystroke to handle.
 * \return The return status of this function.
 */
IK_Status_t IK_KBM_KeystrokeEvent(IK_Keystroke_t keystroke);

/**
 * \brief Starts the transmission of a new keyboard key-state report to the USB
 *        host.
 * \return The return status of this function.
 */
IK_Status_t IK_KBM_SendHIDReport(void);

#endif // __INFINIKEYS_KBMATRIX_H
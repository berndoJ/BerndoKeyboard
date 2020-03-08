// infinikeys_kbmatrix.c
// Copyright (c) 2020 Johannes Berndorfer (berndoJ)

#include "infinikeys_kbmatrix.h"
#include "infinikeys_cfg.h"
#include "infinikeys_usb.h"
#include "infinikeys_debug.h"
#include <stdlib.h>
#include <string.h>

static const IK_CFG_KeyLookupItem_t _kbm_cfg_keyboardmatrix_kltitem =
{
    IK_KBM_KEYBOARD_MATRIX_CFG_KEY,
    IK_CFG_MAX_CFG_VALUE_SIZE,
    IK_CFG_VALUE_TYPE_BYTE_ARRAY
};

//static IK_KBM_Init_t *_kbm_init;
static uint8_t _cur_keylayer = 0U;
static uint16_t _klsw_keycode = 0U;
static uint8_t _hid_pressed_bitmap[32] = {0};
static uint8_t _hid_modifier_byte = 0U;

static void _IK_KBM_HID_SetKeyPressed(uint8_t hid_code);
static void _IK_KBM_HID_SetKeyReleased(uint8_t hid_code);
static void _IK_KBM_HID_SetModifierPressed(uint8_t modifier);
static void _IK_KBM_HID_SetModifierReleased(uint8_t modifier);

IK_Status_t IK_KBM_Init()
{
    // Check for init config null pointer.
    //if (init_cfg == NULL)
    //    return IK_ERROR;

    // Set the configuration.
    //_kbm_init = init_cfg;

    // Add the configuration key lookup table entries for this module.
    IK_CFG_KLT_AddItem(_kbm_cfg_keyboardmatrix_kltitem);

    return IK_OK;
}

IK_Status_t IK_KBM_DeInit(void)
{
    // Reset the configuration.
    //_kbm_init = NULL;

    return IK_OK;
}

/*--------------------------------------------------------------------------------------------------------------------*/

IK_Key_t IK_KBM_RetrieveKeyInfo(uint16_t key_code, uint8_t key_layer)
{
    IK_CFG_Entry_t kcfg;
    IK_Key_t k;
    uint16_t midx = 0, klsize = 0, i;
    uint8_t kl, *kbm = NULL;

    // Check that key_code is not 0.
    if (key_code == 0)
        return (IK_Key_t) IK_KEY_NULL;
    
    // Check the config for the matrix config.
    kcfg = IK_CFG_GetConfig(IK_KBM_KEYBOARD_MATRIX_CFG_KEY);

    // Check if the config returned is valid.
    if (kcfg.Key == 0 || kcfg.ValueSize == 0)
        return (IK_Key_t) IK_KEY_NULL;
    
    kbm = (uint8_t *) kcfg.Value;

    // Check for null-pointer in kbm.
    if (kbm == NULL)
        return (IK_Key_t) IK_KEY_NULL;
    
    // Check the matrix for the key item.
    while (midx < kcfg.ValueSize - 3)
    {
        kl = kbm[midx++];
        klsize = kbm[midx++];
        klsize += ((uint16_t) kbm[midx++]) << 8;

        // Iterate over the elements.
        for (i = 0; i < klsize && midx < kcfg.ValueSize; i++)
        {
            if (kl == key_layer)
            {
                k.KeyCode = (uint16_t) kbm[midx++];
                k.KeyCode += ((uint16_t) kbm[midx++]) << 8;

                k.KeyType = (IK_KeyType_t) kbm[midx++];

                k.KeyData = (uint32_t) kbm[midx++];
                k.KeyData += ((uint32_t) kbm[midx++]) << 8;
                k.KeyData += ((uint32_t) kbm[midx++]) << 16;
                k.KeyData += ((uint32_t) kbm[midx++]) << 24;

                if (k.KeyCode == key_code)
                {
                    return k;
                }
            }
        }
    }

    // Nothing found.
    return (IK_Key_t) IK_KEY_NULL;
}

IK_Status_t IK_KBM_KeystrokeEvent(IK_Keystroke_t keystroke)
{
    IK_Key_t k;

    // Print verbose invoke output.
    #if defined(IK_DEBUG_VERBOSE)
    IK_DEBUG_PrintMessage("KeystrokeEvent INVOKED: K: %i T: %i.\n", keystroke.KeyCode, keystroke.Type);
    #endif // IK_DEBUG_VERBOSE

    // Retrieve information on the key which the event refers to.
    k = IK_KBM_RetrieveKeyInfo(keystroke.KeyCode, _cur_keylayer);
    
    // Check if the key returned is valid.
    if (k.KeyCode == 0 || k.KeyCode != keystroke.KeyCode)
        return IK_OK; // Non-valid keycode, but not an error.
    
    // Check if the keystroke refers to a key which changed the key layer.
    if (_klsw_keycode == k.KeyCode && _cur_keylayer != 0)
    {
        if (keystroke.Type == IK_KEYSTROKE_TYPE_RELEASE)
        {
            _cur_keylayer = 0;
            _klsw_keycode = 0;
        }
        return IK_OK;        
    }

    // Take action according to the information gathered and the keystroke type.
    switch (k.KeyType)
    {
        case IK_KEYTYPE_HID:
            if (keystroke.Type == IK_KEYSTROKE_TYPE_PRESS)
                _IK_KBM_HID_SetKeyPressed(IK_BYTE0(k.KeyData));
            else if (keystroke.Type == IK_KEYSTROKE_TYPE_RELEASE)
                _IK_KBM_HID_SetKeyReleased(IK_BYTE0(k.KeyData));
            return IK_OK;
        case IK_KEYTYPE_HID_MODIFIER:
            if (keystroke.Type == IK_KEYSTROKE_TYPE_PRESS)
                _IK_KBM_HID_SetModifierPressed(IK_BYTE0(k.KeyData));
            else if (keystroke.Type == IK_KEYSTROKE_TYPE_RELEASE)
                _IK_KBM_HID_SetModifierReleased(IK_BYTE0(k.KeyData));
            return IK_OK;
        case IK_KEYTYPE_KEY_LAYER_SWITCH:
            // Handle the press of a keylayer switch on keylayer 0.
            if (keystroke.Type == IK_KEYSTROKE_TYPE_PRESS && _cur_keylayer == 0)
            {
                _cur_keylayer = IK_BYTE0(k.KeyData);
                _klsw_keycode = k.KeyCode;
            }
            return IK_OK;
        case IK_KEYTYPE_ACTION:
            if (keystroke.Type == IK_KEYSTROKE_TYPE_PRESS)
            {
                // Only interested at key-press event.
                // !TODO: Implement key actions.
            }
            return IK_OK;
        case IK_KEYTYPE_NONE: // IK_KEYTYPE_NONE and default are handled same.
        default:
            // Nothing to handle, return OK.
            return IK_OK;
    }
}

IK_Status_t IK_KBM_SendHIDReport(void)
{
    IK_Buffer_t buf;
    IK_Status_t rstat;
    uint8_t i, j, k = 0;

    switch (IK_USB_GetHIDProtocol())
    {
        case IK_USB_HID_REPORT_PROTOCOL_NKRO:
            // Init the buffer to the size of 33 bytes.
            buf.Size = 33; // 1 for modifier byte, 32 for key bytes.
            buf.DataPtr = (uint8_t *) malloc(buf.Size);

            // Check for successful allocation of memory for the buffer.
            if (buf.DataPtr == NULL)
                return IK_ERROR;

            // Fill the buffer with data.
            buf.DataPtr[0] = _hid_modifier_byte;
            memcpy(buf.DataPtr + 1, _hid_pressed_bitmap, 32);

            // Transmit report.
            rstat = IK_USB_SendHIDReport(2U, buf);

            // Free the memory of the buffer.
            free(buf.DataPtr);
            break;
        case IK_USB_HID_REPORT_PROTOCOL_BOOT:
        default:
            // Init the buffer to the size of 8 bytes.
            buf.Size = 8; // 1 modifier byte, 1 reserved byte, 6 HID key-code bytes.
            buf.DataPtr = (uint8_t *) malloc(buf.Size);
            memset(buf.DataPtr, 0x00, buf.Size);

            // Check for successful allocation of memory for the buffer.
            if (buf.DataPtr == NULL)
                return IK_ERROR;
            
            // Fill the modifier and reserved byte.
            buf.DataPtr[0] = _hid_modifier_byte;
            buf.DataPtr[1] = 0x00; // Reserved byte -> set to 0x00

            // Fill the 6 HID key-code bytes.
            for (i = 0; i < 32; i++)
            {
                for (j = 0; j < 8; j++)
                {
                    if (((_hid_pressed_bitmap[i] >> j) & 0x01) == 1U)
                    {
                        buf.DataPtr[2 + (k++)] = (i * 8) + j;
                    }

                    if (k == 6)
                    {
                        goto __loop_exit;
                    }
                }
            }

            __loop_exit:

            // Transmit report.
            rstat = IK_USB_SendHIDReport(1U, buf);

            // Free the buffer.
            free(buf.DataPtr);

            break;
    }

    return rstat;
}

/*--------------------------------------------------------------------------------------------------------------------*/

static void _IK_KBM_HID_SetKeyPressed(uint8_t hid_code)
{
    // Set the corresponding bit on the bitmap to 1.
    _hid_pressed_bitmap[hid_code / 8U] |= 1U << (hid_code % 8U);
}

static void _IK_KBM_HID_SetKeyReleased(uint8_t hid_code)
{  
    // Set the corresponding bit on the bitmap to 0.
    _hid_pressed_bitmap[hid_code / 8U] &= ~(1U << (hid_code % 8U));
}

static void _IK_KBM_HID_SetModifierPressed(uint8_t modifier)
{
    // Set the corresponding bit on the hid modifier byte to 1.
    if (modifier > 7)
        return;
    _hid_modifier_byte |= 1U << modifier;
}

static void _IK_KBM_HID_SetModifierReleased(uint8_t modifier)
{
    // Set the corresponding bit on the hid modifiery byte to 0.
    if (modifier > 7)
        return;
    _hid_modifier_byte &= ~(1U << modifier);
}
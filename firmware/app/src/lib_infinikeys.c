#include "lib_infinikeys.h"

SYS_InitStatus_t lib_infinikeys_init_status = SYS_INIT_STATUS_NOT_INITIALISED;

IK_Init_t lib_infinikeys_init_struct =
{
    .USB_Init = (IK_USB_Init_t)
    {
        .USB_HW_Interface = &infinikeys_hwif_berndokb_usb,
        .USB_VID = 1155U,
        .USB_PID = 4U, //1025U,
        .USB_LangID = 1033U,
        .Manufacturer_Name = "Berndorfer Electronics",
        .Product_Name = "BerndoKeyboard-TESTFW-08-03-B0005",
        .Serial_Number_String = "SN-000001"
    },
    .Config_Init = (IK_CFG_Init_t)
    {
        .ConfigMem_HW_Interface = &infinikeys_hwif_berndokb_cfgmem
    },
    .SWACQ_Init = (IK_SWACQ_Init_t)
    {
        .KBM_HW_Interface = &infinikeys_hwif_berndokb_kbmatrix
    },
    .DEBUG_Init = (IK_DEBUG_Init_t)
    {
        .DEBUG_HW_Interface = &infinikeys_hwif_berndokb_debug
    }
};

static uint8_t _ikmatrix_cfg_tmp[10] =
{
    0x00,                   // Key Layer: 0x00
    0x01, 0x00,             // Key Layer Size: 0x0001
    0x02, 0x00,             //    [0] Key Code: 0x0002
    0x01,                   //    [0] Key Type: HID
    0x04, 0x00, 0x00, 0x00, //    [0] Key Data: 0x00000004 (Key A)
};

void LIB_Infinikeys_Init(void)
{
    IK_CFG_Entry_t e;

    // Init the library.
    if (IK_Init(&lib_infinikeys_init_struct) != IK_OK)
        return;
    
    e.Key = IK_KBM_KEYBOARD_MATRIX_CFG_KEY;
    e.ValueSize = 10;
    e.Value = _ikmatrix_cfg_tmp;

    IK_CFG_SetConfig(e);

    lib_infinikeys_init_status = SYS_INIT_STATUS_INITIALISED;
}
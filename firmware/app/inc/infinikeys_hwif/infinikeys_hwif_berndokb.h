/*******************************************************************************
* \file infinikeys_hwif_berndokb.h
* \date 29.02.2020
********************************************************************************
* \author Johannes Berndorfer (berndoJ)
* \copyright Copyright (c) 2020 by Johannes Berndorfer (berndoJ)
********************************************************************************
* \brief Hardware interface for BerndoKeyboard project.
*******************************************************************************/

#if !defined(__INFINIKEYS_HWIF_BERNDOKB_H)
#define __INFINIKEYS_HWIF_BERNDOKB_H

#include "infinikeys.h"
#include "stm32f1xx_hal.h"

extern IK_USB_HW_Interface_t infinikeys_hwif_berndokb_usb;
extern IK_DEBUG_HW_Interface_t infinikeys_hwif_berndokb_debug;
extern IK_CFG_Memory_HW_Interface_t infinikeys_hwif_berndokb_cfgmem;
extern IK_KBMatrix_HW_Interface_t infinikeys_hwif_berndokb_kbmatrix;

#endif // __INFINIKEYS_HWIF_BERNDOKB_H
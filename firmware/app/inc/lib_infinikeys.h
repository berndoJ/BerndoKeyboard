/*******************************************************************************
* \file lib_infinikeys.h
* \date 29.02.2020
********************************************************************************
* \author Johannes Berndorfer (berndoJ)
* \copyright Copyright (c) 2020 by Johannes Berndorfer (berndoJ)
********************************************************************************
* \brief Wrapper module for InfiniKeys library.
*******************************************************************************/

#if !defined(__LIB_INFINIKEYS_H)
#define __LIB_INFINIKEYS_H

#include "sys_def.h"
#include "infinikeys.h"
#include "infinikeys_hwif/infinikeys_hwif_berndokb.h"

extern SYS_InitStatus_t lib_infinikeys_init_status;
extern IK_Init_t lib_infinikeys_init_struct;

void LIB_Infinikeys_Init(void);

#endif // __LIB_INFINIKEYS_H
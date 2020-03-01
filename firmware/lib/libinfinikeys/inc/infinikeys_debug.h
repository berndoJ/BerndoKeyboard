/*******************************************************************************
* \file infinikeys_debug.h
* \date 29.02.2020
********************************************************************************
* \author Johannes Berndorfer (berndoJ)
* \copyright Copyright (c) 2020 Johannes Berndorfer (berndoJ)
********************************************************************************
* \brief Debug module for InfiniKeys.
*******************************************************************************/

#if !defined(__INFINIKEYS_DEBUG_H)
#define __INFINIKEYS_DEBUG_H

#include "infinikeys_def.h"
#include "infinikeys_hwif.h"

#define IK_DEBUG_MESSAGE_PREFIX     "[IK] "

#define IK_DEBUG_VERBOSE
//#undef IK_DEBUG_VERBOSE // Comment out this line to enable verbose debug output.
#define IK_DEBUG_TICK_VERBOSE
#undef IK_DEBUG_TICK_VERBOSE // Comment out this line to enable tick-verbose debug output.

typedef struct _IK_DEBUG_Init
{
    IK_DEBUG_HW_Interface_t *DEBUG_HW_Interface;
} IK_DEBUG_Init_t;

IK_Status_t IK_DEBUG_Init(IK_DEBUG_Init_t *init_struct);
IK_Status_t IK_DEBUG_DeInit(void);

IK_Status_t IK_DEBUG_PrintMessage(const char* msg, ...);

#endif // __INFINIKEYS_DEBUG_H
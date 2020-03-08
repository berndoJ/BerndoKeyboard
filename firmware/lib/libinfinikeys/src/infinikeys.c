// infinikeys.c
// Copyright (c) 2020 Johannes Berndorfer (berndoJ)

#include "infinikeys.h"

static IK_Init_t *_init;

IK_Status_t IK_Init(IK_Init_t *init_struct)
{
    if (init_struct == NULL)
        return IK_ERROR;

    _init = init_struct;

    // Initialise the modules. If any init struct pointer within _init is NULL,
    // the init function of the corresponding module will return IK_ERROR. Thus
    // NULL-checking these variables is not mandatory in this init function. 
    if (IK_USB_Init(&_init->USB_Init) != IK_OK ||
        IK_CFG_Init(&_init->Config_Init) != IK_OK ||
        IK_KBM_Init() != IK_OK ||
        IK_SWACQ_Init(&_init->SWACQ_Init) != IK_OK ||
        IK_DEBUG_Init(&_init->DEBUG_Init) != IK_OK)
        return IK_ERROR;

    IK_DEBUG_PrintMessage("Initialised Infini-Keys.\n");

    return IK_OK;
}

void IK_DeInit(void)
{
    IK_DEBUG_PrintMessage("De-initialising Infini-Keys...\n");

    // De-init all modules.
    IK_DEBUG_DeInit();
    IK_SWACQ_DeInit();
    IK_KBM_DeInit();
    IK_CFG_DeInit();
    IK_USB_DeInit();
}

void IK_Tick(void)
{
    #if defined(IK_DEBUG_TICK_VERBOSE)
    IK_DEBUG_PrintMessage("Invoked TICK.\n");
    #endif // IK_DEBUG_VERBOSE
    // Invoke tick functions of the submodules.
    IK_CFG_Tick();
    // Invoke tick for polling the keyboard matrix.
    IK_SWACQ_PollTick();
}
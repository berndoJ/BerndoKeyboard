// infinikeys_debug.c
// Copyright (c) 2020 Johannes Berndorfer (berndoJ)

#include "infinikeys_debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

static IK_DEBUG_Init_t *_debug_init;

IK_Status_t IK_DEBUG_Init(IK_DEBUG_Init_t *init_struct)
{
    // Check if the given struct is valid.
    if (init_struct == NULL || init_struct->DEBUG_HW_Interface == NULL)
        return IK_ERROR;
    
    // Set the init struct.
    _debug_init = init_struct;

    return IK_OK;
}

IK_Status_t IK_DEBUG_DeInit(void)
{
    // Clear the init struct.
    _debug_init = NULL;

    return IK_OK;
}

IK_Status_t IK_DEBUG_PrintMessage(const char* msg, ...)
{
    va_list args;
    IK_Status_t rstat;
    uint32_t flen;
    char *fmsg;

    // Check if init and hw interface are set.
    if (_debug_init == NULL || _debug_init->DEBUG_HW_Interface == NULL ||
        _debug_init->DEBUG_HW_Interface->PrintMessage == NULL)
        return IK_ERROR;

    // Allocate the buffer for fmsg.
    fmsg = (char *) malloc(strlen(msg) + 256);

    if (fmsg == NULL)
        return IK_ERROR;

    // sprintf the message with format to fmsg.
    va_start(args, msg);
    flen = (uint32_t) vsprintf(fmsg, msg, args);
    va_end(args);

    // Print to HW interface.
    _debug_init->DEBUG_HW_Interface->PrintMessage(IK_DEBUG_MESSAGE_PREFIX, sizeof(IK_DEBUG_MESSAGE_PREFIX));
    rstat = _debug_init->DEBUG_HW_Interface->PrintMessage(fmsg, flen);

    // De-allocate memory for message.
    free(fmsg);

    return rstat;
}
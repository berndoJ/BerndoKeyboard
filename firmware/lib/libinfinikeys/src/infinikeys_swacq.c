// infinikeys_swacq.c
// Copyright (c) 2020 Johannes Berndorfer (berndoJ)

#include "infinikeys_swacq.h"
#include <stdlib.h>
#include <string.h>
#include "infinikeys_kbmatrix.h"
#include "infinikeys_debug.h"

static IK_SWACQ_Init_t *_swacq_init = NULL;
static IK_KeyState_t *_swacq_ks_buf = NULL;
static uint16_t _swacq_ks_buf_cur_size = 0;

IK_Status_t IK_SWACQ_Init(IK_SWACQ_Init_t *init_struct)
{
    if (init_struct == NULL)
        return IK_ERROR;

    if (init_struct->KBM_HW_Interface == NULL)
        return IK_ERROR;

    _swacq_init = init_struct;

    // Allocate keystate buffer.
    if (IK_SWACQ_AllocKeyStateBuffer(IK_SWACQ_DEFAULT_STATE_BUFFER_SIZE) != IK_OK)
        return IK_ERROR;

    // Check if the keystate buffer allocation was successful or not.
    if (_swacq_ks_buf == NULL)
        return IK_ERROR;

    return IK_OK;
}

IK_Status_t IK_SWACQ_DeInit(void)
{
    // Free the keystate buffer.
    IK_SWACQ_FreeKeyStateBuffer();

    return IK_OK;
}

IK_Status_t IK_SWACQ_PollTick(void)
{
    // Start the HW-interface matrix poll.
    _swacq_init->KBM_HW_Interface->PollMatrix();

    return IK_OK;
}

IK_Status_t IK_SWACQ_AllocKeyStateBuffer(uint16_t initial_size)
{
    uint32_t buf_byte_size;

    _swacq_ks_buf_cur_size = initial_size;
    buf_byte_size = _swacq_ks_buf_cur_size * sizeof(IK_KeyState_t);
    _swacq_ks_buf = (IK_KeyState_t *) malloc(buf_byte_size);

    // Set the allocated space to 0x00.
    memset(_swacq_ks_buf, 0x00, buf_byte_size);
    
    return _swacq_ks_buf == NULL ? IK_ERROR : IK_OK;
}

IK_Status_t IK_SWACQ_ReAllocKeyStateBuffer(uint16_t new_size)
{
    uint16_t old_size = _swacq_ks_buf_cur_size;

    if (new_size == 0)
        return IK_SWACQ_FreeKeyStateBuffer();

    _swacq_ks_buf_cur_size = new_size;
    _swacq_ks_buf = (IK_KeyState_t *) realloc(_swacq_ks_buf, _swacq_ks_buf_cur_size * sizeof(IK_KeyState_t));

    if (_swacq_ks_buf == NULL)
        return IK_ERROR;

    // If a new area was allocated, set the new block to 0x00.
    if (_swacq_ks_buf_cur_size > old_size)
    {
        memset(_swacq_ks_buf + (old_size * sizeof(IK_KeyState_t)), 0x00, (_swacq_ks_buf_cur_size - old_size) * sizeof(IK_KeyState_t));
    }

    return IK_OK;
}

IK_Status_t IK_SWACQ_FreeKeyStateBuffer(void)
{
    free(_swacq_ks_buf);
    return IK_OK;
}

IK_KeyState_t* IK_SWACQ_GetBufferedKeyState(uint16_t key_code)
{
    uint16_t i;
    IK_KeyState_t *ks = NULL;

    if (key_code == 0 || _swacq_ks_buf == NULL)
        return NULL;

    for (i = 0; i < _swacq_ks_buf_cur_size; i++)
    {
        ks = &_swacq_ks_buf[i];

        if (ks->KeyCode == key_code)
            return ks;
    }

    return NULL;
}

IK_KeyState_t* IK_SWACQ_GetInitialisedBufferedKeyState(uint16_t key_code)
{
    uint16_t i, j = 0;
    IK_KeyState_t *ks;

    if (key_code == 0 || _swacq_ks_buf == NULL)
        return NULL;

    ks = IK_SWACQ_GetBufferedKeyState(key_code);

    // The keystate has already been initialised -> return the initialised state
    if (ks != NULL)
        return ks;

    do
    {
        // Get a new slot for the key state.
        for (i = 0; i < _swacq_ks_buf_cur_size; i++)
        {
            ks = &_swacq_ks_buf[i];

            if (ks->KeyCode == 0)
            {
                // Init the keystate.
                ks->KeyCode = key_code;
                ks->Pressed = FALSE;

                return ks;
            }
        }

        if (j > 0)
            break;

        // No free slot found -> reallocate new memory for the buffer.
        if (IK_SWACQ_ReAllocKeyStateBuffer(_swacq_ks_buf_cur_size + IK_SWACQ_DEFAULT_STATE_BUFFER_REALLOC_INCR) != IK_OK)
            return NULL;
        
        // Increase loop index variable.
        j++;
    }
    while (j == 0);

    return NULL;
}

IK_Status_t IK_SWACQ_PollStatusReportCallback(IK_KeyState_t state)
{
    IK_KeyState_t *cur_ks;
    IK_Keystroke_t stroke;

    // Print verbose invoke output.
    #if defined(IK_DEBUG_TICK_VERBOSE)
    IK_DEBUG_PrintMessage("PollStatRep INVOKED: K: %i P: %i.\n", state.KeyCode, state.Pressed);
    #endif // IK_DEBUG_VERBOSE

    // Check for valid key code
    if (state.KeyCode == 0)
        return IK_ERROR;

    // Compare with current key-state.
    cur_ks = IK_SWACQ_GetInitialisedBufferedKeyState(state.KeyCode);

    // Check if valid pointer.
    if (cur_ks == NULL)
        return IK_ERROR;

    if (cur_ks->Pressed != state.Pressed)
    {
        // Key state changed.
        cur_ks->Pressed = state.Pressed;
        stroke.KeyCode = state.KeyCode;
        stroke.Type = state.Pressed == TRUE ? IK_KEYSTROKE_TYPE_PRESS : IK_KEYSTROKE_TYPE_RELEASE;

        // Issue key change event to the KBM module.
        IK_KBM_KeystrokeEvent(stroke);
    }

    // Print verbose termination output.
    #if defined(IK_DEBUG_TICK_VERBOSE)
    IK_DEBUG_PrintMessage("PollStatRep TERMINATED.\n");
    #endif // IK_DEBUG_VERBOSE

    return IK_OK;
}
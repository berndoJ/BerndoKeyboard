/*******************************************************************************
* \file infinikeys_swacq.h
* \date 26.02.2020
********************************************************************************
* \author Johannes Berndorfer (berndoJ)
* \copyright Copyright (c) 2020 Johannes Berndorfer (berndoJ)
********************************************************************************
* \brief Keyboard switch activation state acquisition module.
*******************************************************************************/

#if !defined(__INFINIKEYS_SWACQ_H)
#define __INFINIKEYS_SWACQ_H

#include "infinikeys_def.h"
#include "infinikeys_hwif.h"

#define IK_SWACQ_DEFAULT_STATE_BUFFER_SIZE              128U
#define IK_SWACQ_DEFAULT_STATE_BUFFER_REALLOC_INCR      16U

/**
 * \brief Represents a current key state of a keyboard key.
 */
typedef struct _IK_KeyState
{
    uint16_t KeyCode; /**< The key code of the key that got pressed. */

    IK_Bool_t Pressed; /**< Whether or not the key is pressed or not. */
} IK_KeyState_t;

/**
 * \brief Initialisation struct for this module.
 */
typedef struct _IK_SWACQ_Init
{
    IK_KBMatrix_HW_Interface_t *KBM_HW_Interface; /**< The hardware interface
                                                       for the keyboard
                                                       matrix. */
} IK_SWACQ_Init_t;

/**
 * \brief Initialisation function for this module.
 * \param init_struct Pointer to the initialisation struct used to init this
 *                    module.
 * \return The exit status of this function.
 */
IK_Status_t IK_SWACQ_Init(IK_SWACQ_Init_t *init_struct);

/**
 * \brief De-initialisation function for this module.
 * \return The exit status of this function.
 */
IK_Status_t IK_SWACQ_DeInit(void);

/**
 * \brief Periodic tick update and poll function for this module. This function
 *        invokes the hardware-poll of the keyboard matrix.
 * \return The exit status of this function.
 */
IK_Status_t IK_SWACQ_PollTick(void);

/**
 * \brief Allocates the key state buffer.
 * \param initial_size The initial size (in elements) of the buffer to allocate.
 * \return The exit status of this function.
 */
IK_Status_t IK_SWACQ_AllocKeyStateBuffer(uint16_t initial_size);

/**
 * \brief Re-allocates the key state buffer to the given new size.
 * \param new_size The new size (in elements) the buffer should have.
 * \return The exit status of this function.
 */
IK_Status_t IK_SWACQ_ReAllocKeyStateBuffer(uint16_t new_size);

/**
 * \brief Frees up the allocated key state buffer memory.
 * \return The exit status of this function.
 */
IK_Status_t IK_SWACQ_FreeKeyStateBuffer(void);

/**
 * \brief Retrieves a pointer to a buffered key state of a key given by the
 *        key_code parameter. Returns a NULL-pointer if the requested key state
 *        is not buffered in the key state buffer.
 * \param key_code The key code of the key to get the buffered state of.
 * \return The exit status of this function.
 */
IK_KeyState_t* IK_SWACQ_GetBufferedKeyState(uint16_t key_code);

/**
 * \brief Retrieves a pointer to a buffered key state of a key given by the
 *        key_code parameter. If the requested key state is not currently
 *        buffered in the key state buffer, this function initialises a new
 *        entry in the key state buffer and returns a pointer to this new 
 *        buffer entry. If the key state buffer is full, this function invokes
 *        a re-allocation of the key state buffer memory. Returns NULL if the
 *        key code given is 0 or the re-allocation failed.
 * \param key_code The key code of the key to get the buffered state of.
 * \return The exit status of this function.
 */
IK_KeyState_t* IK_SWACQ_GetInitialisedBufferedKeyState(uint16_t key_code);

/**
 * \brief Callback function which should be called by the HW interface to
 *        report a read keyboard matrix key state.
 * \param state The state to report.
 * \return The exit status of this function.
 */
IK_Status_t IK_SWACQ_PollStatusReportCallback(IK_KeyState_t state);

#endif // __INFINIKEYS_SWACQ_H
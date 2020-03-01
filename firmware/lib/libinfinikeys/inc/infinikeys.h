/*******************************************************************************
* \file infinikeys.h
* \date 14.01.2020
********************************************************************************
* \author Johannes Berndorfer (berndoJ)
* \copyright Copyright (c) 2020 Johannes Berndorfer (berndoJ)
********************************************************************************
* \brief Main header file for libinfinikeys.
*******************************************************************************/

#if !defined(__INFINIKEYS_H)
#define __INFINIKEYS_H

#include "infinikeys_def.h"
#include "infinikeys_hwif.h"
#include "infinikeys_usb.h"
#include "infinikeys_cfg.h"
#include "infinikeys_kbmatrix.h"
#include "infinikeys_swacq.h"
#include "infinikeys_debug.h"

/**
 * \brief Initialisation data struct which holds the necessary information for
 *        initialising the InfiniKeys library.
 */
typedef struct _IK_Init
{
    IK_USB_Init_t USB_Init; /**< The initialisation configuration for the
                                 InfiniKeys-USB module. */

    IK_CFG_Init_t Config_Init; /**< The initialisation configuration for the
                                    Infinikeys-Config module. */

    IK_SWACQ_Init_t SWACQ_Init; /**< The intialisation configuration for the 
                                     Infinikeys-SWACQ module. */

    IK_DEBUG_Init_t DEBUG_Init; /**< The intialisation configuration for the
                                     Infinikeys-Debug module. */
} IK_Init_t;


/**
 * \brief Initialises the whole InfiniKeys library and all submodules with the
 *        given initialisation structure.
 * \param init_struct The initialisation struct for supplying the initialisation
 *                    function with the necessary init information.
 * \return The return status of the function.
 */
IK_Status_t IK_Init(IK_Init_t *init_struct);

/**
 * \brief De-initialises the whole InfiniKeys library and all submodules.
 */
void IK_DeInit(void);

/**
 * \brief Executes the code which should be executed regularly with non-constant
 *        interval.
 */
void IK_Tick(void);

#endif // __INFINIKEYS_H
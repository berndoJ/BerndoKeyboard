/*******************************************************************************
* \file sys_def.h
* \date 04.01.2020
********************************************************************************
* \author Johannes Berndorfer (berndoJ)
* \copyright Copyright (c) 2020 by Johannes Berndorfer (berndoJ)
********************************************************************************
* \brief Header for common definitions within this application.
*******************************************************************************/

#if !defined(__SYS_DEF_H)
#define __SYS_DEF_H

/**
 * \defgroup ModulesActived Activated firmware modules.
 * @{
 */

#define __SYS_ENABLE_LIGHTING_DRIVER        1
#define __SYS_ENABLE_USB_DRIVER             1
#define __SYS_ENABLE_PORTEX_DRIVER          1
#define __SYS_ENABLE_INFINIKEYS             1

/**@}*/

#if (__SYS_ENABLE_INFINIKEYS == 1 && __SYS_ENABLE_PORTEX_DRIVER != 1)
#error Infinikeys requires the PCA9555 port-expander driver to function.
#endif

/**
 * \brief Represents the intialisation status of some kind of structure.
 */
typedef enum
{
    SYS_INIT_STATUS_NOT_INITIALISED, /**< The underlying construct is not yet initialised. */
    SYS_INIT_STATUS_INITIALISED /**< The underlying construct is initialised and ready. */
} SYS_InitStatus_t;

#endif // __SYS_DEF_H
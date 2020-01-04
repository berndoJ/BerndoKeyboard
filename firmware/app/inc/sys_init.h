/*******************************************************************************
* \file sys_init.h
* \date 03.01.2020
********************************************************************************
* \author Johannes Berndorfer (berndoJ)
* \copyright Copyright (c) 2020 by Johannes Berndorfer (berndoJ)
********************************************************************************
* \brief Initialisation module of this application. This module contains init
*        code to run after the main function has been invoked by the start
*        script.
*******************************************************************************/

#if !defined(__SYS_INIT_H)
#define __SYS_INIT_H

/**
 * \brief Initialises the application after the main function has been invoked.
 */
void SYS_Init(void);

#endif // __SYS_INIT_H
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

/**
 * \brief Initialisation function that gets called after all ports have been initialised and configured by the main
 *        function.
 */
void SYS_PostPortInit(void);

/**
 * \brief Initialisation function that gets called after all libs have been initialised by the main function.
 */ 
void SYS_PostLibInit(void);

#endif // __SYS_INIT_H
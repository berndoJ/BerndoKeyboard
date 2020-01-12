/*******************************************************************************
* \file debug.h
* \date 11.01.2020
********************************************************************************
* \author Johannes Berndorfer (berndoJ)
* \copyright Copyright (c) 2020 by Johannes Berndorfer (berndoJ)
********************************************************************************
* \brief Debug interface information provider.
*******************************************************************************/

#if !defined(__DEBUG_H)
#define __DEBUG_H

#include <stdint.h>

/**
 * \brief Prints a given debug information message to the debug terminal / UART
 *        interface.
 * \param msg The message to print. (Pointer to the char buffer containing the
 *            message, terminated with a 0x00)
 */
void SYS_PrintDebugMessage(char *msg);

void SYS_DumpMemToDebugInterface(void *mem_ptr, uint16_t mem_len);

#endif // __DEBUG_H
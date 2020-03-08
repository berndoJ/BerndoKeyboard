/*******************************************************************************
* \file sys_dma.h
* \date 06.01.2020
********************************************************************************
* \author Johannes Berndorfer (berndoJ)
* \copyright Copyright (c) 2020 by Johannes Berndorfer (berndoJ)
********************************************************************************
* \brief Initialisation module for all DMA streams of the processor.
*******************************************************************************/

#if !defined(__SYS_DMA_H)
#define __SYS_DMA_H

#include "sys_def.h"

/**
 * \brief Initialises the systems DMA streams/channels.
 */
void SYS_DMA_Init(void);

#endif // __SYS_DMA_H
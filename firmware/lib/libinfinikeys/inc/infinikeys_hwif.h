/*******************************************************************************
* \file infinikeys_hwif.h
* \date 28.01.2020
********************************************************************************
* \author Johannes Berndorfer (berndoJ)
* \copyright Copyright (c) 2020 Johannes Berndorfer (berndoJ)
********************************************************************************
* \brief Definition header for all hardware interface related types.
*******************************************************************************/

#if !defined(__INFINIKEYS_HWIF_H)
#define __INFINIKEYS_HWIF_H

#include "infinikeys_def.h"

/**
 * \brief Interface for hardware implementation of the configuration memory
 *        interface.
 */
typedef struct _IK_CFG_Memory_HW_Interface
{
    IK_Status_t (* Init) (void);
    IK_Status_t (* DeInit) (void);
    IK_Status_t (* WriteByte) (uint32_t address, uint8_t data);
    IK_Status_t (* WriteBuffer) (uint32_t address, IK_Buffer_t data_buf);
    uint8_t (* ReadByte) (uint32_t address);
    IK_Buffer_t (* ReadBuffer) (uint32_t address, uint32_t size);

    uint32_t MemorySize;
} IK_CFG_Memory_HW_Interface_t;

/**
 * \brief Struct for holding calls to the hardware interface.
 */
typedef struct _IK_USB_HW_Interface
{
    IK_Status_t (* Init) (void);
    void        (* DeInit) (void);
    IK_Status_t (* TransmitUSB) (IK_Buffer_t buf, uint8_t ep_addr);
} IK_USB_HW_Interface_t;

typedef struct _IK_KBMatrix_HW_Interface
{
    IK_Status_t (* PollMatrix) (void);
} IK_KBMatrix_HW_Interface_t;

typedef struct _IK_DEBUG_HW_Interface
{
    IK_Status_t (* PrintMessage) (const char *msg, uint32_t len);
} IK_DEBUG_HW_Interface_t;

#endif // __INFINIKEYS_HWIF_H
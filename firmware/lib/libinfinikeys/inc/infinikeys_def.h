/*******************************************************************************
* \file infinikeys_def.h
* \date 25.01.2020
********************************************************************************
* \author Johannes Berndorfer (berndoJ)
* \copyright Copyright (c) 2020 Johannes Berndorfer (berndoJ)
********************************************************************************
* \brief Common definitions and includes for InfiniKeys.
*******************************************************************************/

#if !defined(__INFINIKEYS_DEF_H)
#define __INFINIKEYS_DEF_H

#include <stddef.h>
#include <stdint.h>

#if !defined(INFINIKEYS_VER_MAJ)
#define INFINIKEYS_VER_MAJ      0
#endif // INFINIKEYS_VER_MAJ
#if !defined(INFINIKEYS_VER_MIN)
#define INFINIKEYS_VER_MIN      0
#endif // INFINIKEYS_VER_MIN
#if !defined(INFINIKEYS_VER_REV)
#define INFINIKEYS_VER_REV      0
#endif // INFINIKEYS_VER_REV

#define IK_LOWBYTE(x)           ((uint8_t) (x & 0x000000ff))
#define IK_HIGHBYTE(x)          ((uint8_t)((x & 0x0000ff00) >> 8))
#define IK_BYTE2(x)             ((uint8_t)((x & 0x00ff0000) >> 16))
#define IK_BYTE3(x)             ((uint8_t)((x & 0xff000000) >> 24))

#define IK_BYTE0(x)             IK_LOWBYTE(x)
#define IK_BYTE1(x)             IK_HIGHBYTE(x)

#define IK_UNUSED(x)            (void)(x)

#define IK_MIN(a,b)             (a < b ? a : b)
#define IK_MAX(a,b)             (a > b ? a : b)

/**
 * \brief Function return status representation.
 */
typedef enum _IK_Status
{
    IK_OK = 0U, /**< The function executed successfully and no errors occurred
                     during the functions execution. */

    IK_ERROR, /**< During the functions execution an error occurred. The
                   function did not complete its task successfully. */

    IK_BUSY /**< The functions task could not be completed due to something
                 being currently busy executing another task. */
} IK_Status_t;

/**
 * \brief Represents a boolean value.
 */
typedef enum _IK_Bool
{
    FALSE = 0U, /**< Boolean value FALSE, LOW or 0 */

    TRUE = 1U /**< Boolean value TRUE, HIGH or 1 */
} IK_Bool_t;

/**
 * \brief Structur for a unified byte (data) buffer.
 */
typedef struct _IK_Buffer
{
    uint8_t *DataPtr; /**< Pointer to the buffer's data. */

    uint32_t Size; /**< The size (in bytes / elements) of the buffers content. */
} IK_Buffer_t;

#endif // __INFINIKEYS_DEF_H
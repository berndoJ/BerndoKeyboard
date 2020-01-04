/*******************************************************************************
* \file error.h
* \date 03.01.2020
********************************************************************************
* \author Johannes Berndorfer (berndoJ)
* \copyright Copyright (c) 2020 by Johannes Berndorfer (berndoJ)
********************************************************************************
* \brief Provides a simple and unified interface for handling different types of
*        errors in the application.
*******************************************************************************/

#if !defined(__ERROR_H)
#define __ERROR_H

#include "stdint.h"

/**
 * \brief Used to specify the type of an error.
 */
typedef enum {
    SYSERR_WARNING = 0, /**< A warning leads to the output of the warning message to the correct debug interfaces, but will not stop the execution of the program. */
    SYSERR_ERROR = 1, /**< A normal error has the same effect of a warning, but the message printed to the debug interfaces is labeled as an error. */
    SYSERR_FATAL_ERROR = 2 /**< A fatal error leads to the same message of a normal error, but will halt the execution of the program. */
} SYS_ErrorType_t;

/**
 * \brief Throws an error. The effects of this function are determined by the
 *        type of error thrown. (err_type parameter)
 * \param err_type The type of error thrown.
 * \param line_number The line number the error occurred in. (Usually `__LINE__`)
 * \param file_name The name of the file in which the error occurred in. (Usually `__FILE__`)
 * \param err_msg The error message to print to the debug interface(s).
 */
void SYS_ThrowError(SYS_ErrorType_t err_type, uint32_t line_number, const char* file_name, const char* err_msg);

#endif // __ERROR_H
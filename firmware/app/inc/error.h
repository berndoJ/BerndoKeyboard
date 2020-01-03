#if !defined(__ERROR_H)
#define __ERROR_H

#include "stdint.h"

typedef enum {
    SYSERR_WARNING = 0,
    SYSERR_ERROR = 1,
    SYSERR_FATAL_ERROR = 2
} SYS_ErrorType_t;

void SYS_ThrowError(SYS_ErrorType_t err_type, uint32_t line_number, const char* file_name, const char* err_msg);

#endif // __ERROR_H
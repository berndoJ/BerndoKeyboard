#include "error.h"

void SYS_ThrowError(SYS_ErrorType_t err_type, uint32_t line_number, const char* file_name, const char* err_msg)
{
    switch (err_type)
    {
        case SYSERR_WARNING:
        case SYSERR_ERROR:
            //* NON-FATAL ERROR HANDLING
            break;
        case SYSERR_FATAL_ERROR:
            //* FATAL ERROR HANDLING
    	    while (1) {}
            break;
    }
}
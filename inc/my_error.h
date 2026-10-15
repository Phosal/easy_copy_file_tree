#ifndef MY_ERROR_H
#define MY_ERROR_H

#include <wchar.h>

typedef enum
{
    ERROR_PARAMETER_TOO_MANY = 0,
    ERROR_PATH_TOO_LONG,
    ERROR_PATH_NOT_RIGHT,
    ERROR_CLIPBOARD_NO_ACCESS,
    ERROR_MALLOC_WRONG,
    ERROR_FOLDER_EMPTY,
    ERROR_GLOBAL_ALLOC_FAIL,
    ERROR_CREATE_REGI_WRONG,
    ERROR_SET_REGI_WRONG,
    ERROR_ERROR_MSG_COUNT_LESS,
    ERROR_NOTHING
} err_t;

extern err_t error;
err_t translate_error(err_t error, wchar_t *msg, int count);

#endif
#include "my_error.h"

err_t error = ERROR_NOTHING;

err_t translate_error(err_t error, wchar_t *msg, int count)
{
    const wchar_t *error_string[] = {
        L"ERROR_PARAMETER_TOO_MANY",
        L"ERROR_PATH_TOO_LONG",
        L"ERROR_PATH_NOT_RIGHT",
        L"ERROR_CLIPBOARD_NO_ACCESS",
        L"ERROR_MALLOC_WRONG",
        L"ERROR_FOLDER_EMPTY",
        L"ERROR_GLOBAL_ALLOC_FAIL",
        L"ERROR_CREATE_REGI_WRONG",
        L"ERROR_SET_REGI_WRONG",
        L"ERROR_ERROR_MSG_COUNT_LESS",
        L"ERROR_NOTHING"};

    if (count > wcslen(error_string[error]) + 1)
    {
        swprintf(msg, count, L"%ls", error_string[error]);
        return ERROR_NOTHING;
    }
    return ERROR_ERROR_MSG_COUNT_LESS;
}
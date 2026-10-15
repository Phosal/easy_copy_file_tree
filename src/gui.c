#include <windows.h>
#include "gui.h"

err_t write_registry_value(HKEY regi_root_key, LPCWSTR regi_child_key_path, LPCWSTR regi_value_name, LPCWSTR regi_value_data)
{
    HKEY regi_handle = NULL;
    DWORD regi_disposition;
    LONG result;

    result = RegCreateKeyExW(
        regi_root_key,
        regi_child_key_path,
        0,
        NULL,
        REG_OPTION_NON_VOLATILE,
        KEY_WRITE|KEY_READ,
        NULL,
        &regi_handle,
        &regi_disposition);

    if (result != ERROR_SUCCESS)
    {
        return ERROR_CREATE_REGI_WRONG;
    }

    result = RegSetValueExW(
        regi_handle,
        regi_value_name,
        0,
        REG_SZ,
        (const BYTE *)regi_value_data,
        (wcslen(regi_value_data) + 1) * sizeof(wchar_t));

    if (result != ERROR_SUCCESS)
    {
        RegCloseKey(regi_handle);
        return ERROR_SET_REGI_WRONG;
    }
    
    RegCloseKey(regi_handle);
    return ERROR_NOTHING;
}

err_t on_click_config_button(HWND owner_handle)
{
    wchar_t exe_path[MAX_PATH];
    GetModuleFileNameW(NULL, exe_path, MAX_PATH);

    wchar_t regi_command_value[MAX_PATH];
    regi_command_value[0] = L'\"';
    regi_command_value[1] = L'\0';
    wcscat(regi_command_value, exe_path);
    wcscat(regi_command_value, L"\" \"%1\"");

    wchar_t regi_child_key_path[MAX_PATH];
    swprintf_s(regi_child_key_path, MAX_PATH, L"Directory\\shell\\easy_copy_file_tree");

    wchar_t regi_menu_value[20];
    swprintf_s(regi_menu_value, 20, L"复制内部文件树状图");

    err_t error = ERROR_NOTHING;

    error = write_registry_value(
        HKEY_CLASSES_ROOT,
        regi_child_key_path,
        NULL,
        regi_menu_value);

    if (error != ERROR_NOTHING)
    {
        return error;
    }

    wcscat(regi_child_key_path, L"\\command");
    error = write_registry_value(
        HKEY_CLASSES_ROOT,
        regi_child_key_path,
        NULL,
        regi_command_value);
    return error;
}

void on_click_close_button(HWND onwer_handle)
{
    if (MessageBoxW(onwer_handle, L"确认关闭", L"提示", MB_YESNO) == IDYES)
    {
        PostQuitMessage(0);
    }
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
    {
        CreateWindowW(
            L"BUTTON",
            L"配置",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            20, 20, 100, 30,
            hWnd,
            (HMENU)BUTTON_CONFIG,
            GetModuleHandleW(NULL),
            NULL);
        CreateWindowW(
            L"BUTTON",
            L"关闭",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            140, 20, 100, 30,
            hWnd,
            (HMENU)BUTTON_CLOSE,
            GetModuleHandleW(NULL),
            NULL);
        break;
    }
    case WM_COMMAND:
    {
        int wmID = LOWORD(wParam);    // 提取控件ID
        int wmEvent = HIWORD(wParam); // 提取通知码

        if (wmEvent == BN_CLICKED)
        {
            switch (wmID)
            {
            case BUTTON_CONFIG:
            {
                error = on_click_config_button(hWnd);
                wchar_t error_msg[50];
                error = translate_error(error, error_msg, 50);
                if (error != ERROR_NOTHING)
                {
                    MessageBoxW(NULL, L"ERROR_ERROR_MSG_COUNT_LESS", L"错误", MB_OK);
                    PostQuitMessage(0);
                }
                else
                {
                    MessageBoxW(NULL, error_msg, L"错误", MB_OK);
                    PostQuitMessage(0);
                }
                break;
            }
            case BUTTON_CLOSE:
            {
                on_click_close_button(hWnd);
                break;
            }
            }
        }
        break;
    }
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        break;
    }
    default:
    {
        return DefWindowProcW(hWnd, uMsg, wParam, lParam);
    }
    }
    return 0;
}



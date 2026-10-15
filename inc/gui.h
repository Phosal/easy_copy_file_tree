#ifndef GUI_H
#define GUI_H

#define BUTTON_CONFIG 1001
#define BUTTON_CLOSE 1002

#include <windows.h>
#include "my_error.h"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

err_t on_click_config_button(HWND owner_handle);
void on_click_close_button(HWND owner_handle);

err_t write_registry_value(HKEY regi_root_key, LPCWSTR regi_child_key_path, LPCWSTR regi_value_name, LPCWSTR regi_value_data);

#endif
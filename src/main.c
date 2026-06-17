#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <stdbool.h>
#include "../inc/file_tree.h"

char exe_path[MAX_PATH];

int wmain(int argc, wchar_t *argv[])
{
    if (OpenClipboard(NULL))
    {
        wchar_t *wpath = (argc > 1) ? argv[1] : L".";

        wchar_t search_path[MAX_PATH];
        WIN32_FIND_DATAW find_data;
        HANDLE handle_find;

        _snwprintf(search_path, sizeof(search_path), L"%s\\*", wpath);
        file_tree_t file_tree;
        file_tree_t *pfile_tree = &file_tree;
        file_tree_init(pfile_tree);

        for (int i = wcslen(wpath); i > 0; i--)
        {
            if (wpath[i] == L'\\')
            {
                pfile_tree->root = create_new_node(&(wpath[i + 1]), FILE_ATTRIBUTE_DIRECTORY);
                break;
            }
        }

        file_tree_build(pfile_tree,
                        pfile_tree->root,
                        search_path,
                        handle_find,
                        &find_data);

        file_tree_mark(pfile_tree,
                       pfile_tree->root,
                       1,
                       true);
        pfile_tree->print_column_flags = (bool *)malloc(sizeof(bool) * pfile_tree->max_column);
        for (int i = 0; i < pfile_tree->max_column; i++)
        {
            pfile_tree->print_column_flags[i] = true;
        }
        file_tree_string_make(pfile_tree,
                              pfile_tree->root);

        HGLOBAL global_m_handle = GlobalAlloc(GMEM_MOVEABLE, (wcslen(pfile_tree->file_tree_string) + 1) * sizeof(wchar_t));
        if (global_m_handle)
        {
            wchar_t *clipboard_data = (wchar_t *)GlobalLock(global_m_handle);
            wcscpy(clipboard_data, pfile_tree->file_tree_string);
            GlobalUnlock(global_m_handle);
            SetClipboardData(CF_UNICODETEXT, global_m_handle);
        }
    }
    CloseClipboard();

    return 0;
}

// GetModuleFileNameA(NULL,
//                    exe_path,
//                    MAX_PATH);
// HKEY h_key;
// RegCreateKeyExA(HKEY_CURRENT_USER,
//                 "Software\\Classes\\Directory\\Shell\\copy_tree",
//                 0,
//                 NULL,
//                 0,
//                 KEY_WRITE,
//                 NULL,
//                 &h_key,
//                 NULL);
// char *menu_text = "copy_tree";
// RegSetValueExA(h_key,
//                NULL,
//                0,
//                REG_SZ,
//                (BYTE *)menu_text,
//                strlen(menu_text) + 1);

// RegCloseKey(h_key);

// HKEY h_cmd_key;
// RegCreateKeyExA(HKEY_CURRENT_USER,
//                 "Software\\Classes\\Directory\\shell\\copy_tree\\command",
//                 0,
//                 NULL,
//                 0,
//                 KEY_WRITE,
//                 NULL,
//                 &h_cmd_key,
//                 NULL);
// char command[1024] = {0};
// sprintf(command, "\"%s\" \"%%1\"", exe_path);
// RegSetValueExA(h_cmd_key,
//                NULL,
//                0,
//                REG_SZ,
//                (BYTE *)command,
//                strlen(command) + 1);
// RegCloseKey(h_cmd_key);
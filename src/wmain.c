#include <stdio.h>
#include <windows.h>
#include "wmain.h"
#include "gui.h"
#include "my_error.h"

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrev,
                    PWSTR pCmdLine, int nCmdShow)
{
    // 判断程序是双击打开还是调用打开
    int argc = 0;
    wchar_t **argv = CommandLineToArgvW(GetCommandLineW(), &argc);

    if (argc == 1)
    {
    gui:
        if (error != ERROR_NOTHING)
        {
            wchar_t error_msg[50];
            translate_error(error, error_msg, 50);
            MessageBoxW(
                NULL,
                error_msg,
                L"错误",
                MB_OK);
            return 0;
        }

        HINSTANCE exe_handle = hInst;

        const wchar_t CLASS_NAME[] = L"注册窗口类";
        WNDCLASSW wc = {0};
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = exe_handle;
        wc.lpszClassName = CLASS_NAME;

        RegisterClassW(&wc);

        HWND hWnd = CreateWindowExW(
            0,
            CLASS_NAME,
            L"程序配置",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, 350, 150,
            NULL, NULL, exe_handle, NULL);

        if (hWnd == NULL)
        {
            return 0;
        }

        ShowWindow(hWnd, SW_SHOWNORMAL);
        UpdateWindow(hWnd);

        MSG msg = {0};
        while (GetMessageW(&msg, NULL, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }
    else if (argc == 2)
    {
        errno_t err = 0;
        wchar_t folder_path[MAX_PATH];
        err = wcscpy_s(folder_path, MAX_PATH, argv[1]);

        if (err != 0)
        {
            error = ERROR_PATH_TOO_LONG;
            goto free;
        }

        wchar_t search_path[MAX_PATH];
        err = _snwprintf_s(search_path, MAX_PATH, MAX_PATH - 1, L"%s\\*", folder_path);

        if (err == -1)
        {
            error = ERROR_PATH_TOO_LONG;
            goto free;
        }

        // 拼接根文件夹名字
        file_tree_t file_tree;
        file_tree_t *pfile_tree = &file_tree;
        file_tree_init(pfile_tree);
        wchar_t *p = wcsrchr(folder_path, L'\\');
        error = create_new_node(p + 1,
                                FILE_ATTRIBUTE_DIRECTORY,
                                &(pfile_tree->root));
        if (error != ERROR_NOTHING)
        {
            goto free;
        }

        WIN32_FIND_DATAW data_find;
        HANDLE handle_find;

        error = file_tree_build(pfile_tree,
                                pfile_tree->root,
                                search_path,
                                handle_find,
                                &data_find);

        if (error != ERROR_NOTHING)
        {
            goto free;
        }

        file_tree_mark(pfile_tree,
                       pfile_tree->root,
                       1,
                       true);
        pfile_tree->print_column_flags = (bool *)malloc(sizeof(bool) * pfile_tree->max_column);
        if (pfile_tree->print_column_flags == NULL)
        {
            error = ERROR_MALLOC_WRONG;
            goto free;
        }
        for (int i = 0; i < pfile_tree->max_column; i++)
        {
            pfile_tree->print_column_flags[i] = true;
        }
        file_tree_string_make(pfile_tree, pfile_tree->root);

        if (OpenClipboard(NULL) == 0)
        {
            CloseClipboard();
            error = ERROR_CLIPBOARD_NO_ACCESS;
            goto free;
        }

        HGLOBAL global_memory_handle = GlobalAlloc(GMEM_MOVEABLE,
                                                   (wcslen(pfile_tree->file_tree_string) + 0) * sizeof(wchar_t));
        if (global_memory_handle == NULL)
        {
            error = ERROR_GLOBAL_ALLOC_FAIL;
            goto free;
        }
        wchar_t *clipboard_data = (wchar_t *)GlobalLock(global_memory_handle);
        wcscpy(clipboard_data, pfile_tree->file_tree_string);
        GlobalUnlock(global_memory_handle);
        SetClipboardData(CF_UNICODETEXT, global_memory_handle);
        GlobalFree(global_memory_handle);
        CloseClipboard();

        free(pfile_tree->print_column_flags);
        free_everything(pfile_tree->root);
        LocalFree(argv);
        return 0;
    free:
        free(pfile_tree->print_column_flags);
        free_everything(pfile_tree->root);
        LocalFree(argv);
        goto gui;
    }
    else
    {
        return 0;
    }
}

void free_everything(tree_node_t *root)
{
    if (root->next_level_node != NULL)
    {
        free_everything(root->next_level_node);
    }

    if (root->same_level_node != NULL)
    {
        free_everything(root->same_level_node);
    }

    free(root->name);
    free(root);
}

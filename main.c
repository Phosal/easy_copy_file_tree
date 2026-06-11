#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <stdbool.h>
#include <locale.h>
#include "link_list.h"

#define MAX_PATH_LENGTH 260

#define SOFTCORE_PATH

char *path; //= "D:\\Code\\C Code\\root";
WCHAR final_string[1000] = {L'\0'};
int max_column = 0;
bool *if_print_column = NULL;

void build_list(list_node_t *root,
                WCHAR *search_path,
                HANDLE handle_find,
                WIN32_FIND_DATAW *find_data);

void make_mark(list_node_t *root,
               int current_column,
               bool if_my_dad_last_child);

void make_string(list_node_t *root);

int main(int argc, char *argv[]) // 目录最终目标必须是文件夹
{

#ifndef HARDCORE_PATH
    path = (argc > 1) ? argv[1] : ".";
#endif

    if (OpenClipboard(NULL))
    {
        int path_bytes = (strlen(path) + 1) * sizeof(path[0]);
        int wpath_chars = MultiByteToWideChar(CP_UTF8, 0, path, path_bytes, NULL, 0);
        wchar_t *wpath = (wchar_t *)malloc(sizeof(wchar_t) * wpath_chars);
        MultiByteToWideChar(CP_UTF8, 0, path, path_bytes, wpath, wpath_chars);

        setlocale(LC_ALL, "");
        wchar_t search_path[MAX_PATH_LENGTH];
        WIN32_FIND_DATAW find_data;
        HANDLE handle_find;

        _snwprintf(search_path, sizeof(search_path), L"%s\\*", wpath);
        list_node_t *root = NULL;
        for (int i = wcslen(wpath); i > 0; i--)
        {
            if (wpath[i] == L'\\')
            {
                root = create_new_node(&(wpath[i + 1]), FILE_ATTRIBUTE_DIRECTORY);
                break;
            }
        }
        build_list(root,
                   search_path,
                   handle_find,
                   &find_data);

        make_mark(root, 1, true);
        if_print_column = (bool *)malloc(sizeof(bool) * max_column);
        for (int i = 0; i < max_column; i++)
        {
            if_print_column[i] = true;
        }
        make_string(root);

        HGLOBAL global_m_handle = GlobalAlloc(GMEM_MOVEABLE, (wcslen(final_string) + 1) * sizeof(wchar_t));
        if (global_m_handle)
        {
            wchar_t *clipboard_data = (wchar_t *)GlobalLock(global_m_handle);
            wcscpy(clipboard_data, final_string);
            GlobalUnlock(global_m_handle);
            SetClipboardData(CF_UNICODETEXT, global_m_handle);
        }
    }
    CloseClipboard();

    system("pause");
    return 0;
}

void build_list(list_node_t *root,
                WCHAR *search_path,
                HANDLE handle_find,
                WIN32_FIND_DATAW *find_data)
{
    bool if_first_folder = true;
    bool if_first_file = true;
    list_node_t *folder_head = NULL;
    list_node_t *folder_end = NULL;
    list_node_t *file_head = NULL;
    list_node_t *file_end = NULL;

    handle_find = FindFirstFileW(search_path, find_data);

    if (handle_find == INVALID_HANDLE_VALUE)
    {
        // fprintf(stderr, "Error：cannot open directory \"%s\" or cannot find any file in this directory\n", path);
        printf("Error：cannot open directory or cannot find any file in this directory\n");
        root->next_level_node = NULL;
        return;
    }

    do
    {
        if (wcscmp(find_data->cFileName, L".") == 0 ||
            wcscmp(find_data->cFileName, L"..") == 0)
            continue;
        list_node_t *new_node = create_new_node(find_data->cFileName, find_data->dwFileAttributes);
        if (new_node->node_type == IS_FOLDER)
        {
            if (if_first_folder)
            {
                folder_head = new_node;
                folder_end = new_node;
                if_first_folder = false;
            }
            else
            {
                new_node->same_level_node = folder_end->same_level_node;
                folder_end->same_level_node = new_node;
                folder_end = new_node;
            }
        }
        else
        {
            if (if_first_file)
            {
                file_head = new_node;
                file_end = new_node;
                if_first_file = false;
            }
            else
            {
                new_node->same_level_node = file_end->same_level_node;
                file_end->same_level_node = new_node;
                file_end = new_node;
            }
        }
    } while (FindNextFileW(handle_find, find_data));
    // 进入此if证明root文件夹内没有任何文件(.和..文件夹已被过滤)
    if (if_first_folder == true && if_first_file == true)
    {
        root->next_level_node = NULL;
        return;
    }
    // 如果进入下面的if，证明至少有一个文件夹或者至少有一个文件
    if (if_first_folder == true) // 只有文件
    {
        root->next_level_node = file_head;
        return;
    }
    // 如果运行到这一步，证明只有文件夹或者有文件夹也有文件
    folder_end->same_level_node = file_head;
    root->next_level_node = folder_head;

    list_node_t *current_node = root->next_level_node;
    search_path[wcslen(search_path) - 1] = L'\0';
    while (current_node != NULL && current_node->node_type == IS_FOLDER)
    {
        WCHAR search_next_level_path[MAX_PATH_LENGTH];
        _snwprintf(search_next_level_path, sizeof(search_next_level_path), L"%s%s\\*", search_path, current_node->name);
        WIN32_FIND_DATAW find_next_level_data;
        HANDLE handle_next_level_find;
        build_list(current_node, search_next_level_path, handle_next_level_find, &find_next_level_data);
        current_node = current_node->same_level_node;
    }
}

void make_mark(list_node_t *root, int current_column, bool if_my_dad_last_child)
{
    static int current_layer = 1;
    if (current_column > max_column)
    {
        max_column = current_column;
    }
    root->is_my_dad_the_last_child = if_my_dad_last_child;
    root->am_i_the_last_child = (root->same_level_node == NULL) ? true : false;
    root->layer = current_layer++;
    root->column = current_column;
    if (root->next_level_node != NULL)
    {
        make_mark(root->next_level_node, current_column + 1, root->am_i_the_last_child);
    }
    if (root->same_level_node != NULL)
    {
        make_mark(root->same_level_node, current_column, root->is_my_dad_the_last_child);
    }
}

void make_string(list_node_t *root)
{
    if (root->is_my_dad_the_last_child == true)
    {
        if (root->column - 2 > 0)
        {
            if_print_column[root->column - 2] = false;
        }
    }
    for (int column = 1; column < root->column; column++)
    {
        if (column != root->column - 1)
        {
            if (if_print_column[column] == true)
            {
                wcscat(final_string, L"│");
                wcscat(final_string, L"  ");
                // printf("│");
                // printf("  ");
            }
            else
            {
                wcscat(final_string, L" ");
                wcscat(final_string, L"  ");
                // printf(" ");
                // printf("  ");
            }
        }
        else
        {
            if (root->am_i_the_last_child == true)
            {
                wcscat(final_string, L"└");
                wcscat(final_string, L"──");
                // printf("└");
                // printf("──");
            }
            else
            {
                wcscat(final_string, L"├");
                wcscat(final_string, L"──");
                // printf("├");
                // printf("──");
            }
        }
    }
    wcscat(final_string, root->name);
    // printf("%s",root->name);
    if (root->node_type == IS_FOLDER)
    {
        wcscat(final_string, L"/\n");
        // printf("/\n");
    }
    else
    {
        wcscat(final_string, L"\n");
        // printf("\n");
    }

    if (root->next_level_node != NULL)
    {
        make_string(root->next_level_node);
    }
    if (root->same_level_node != NULL)
    {
        make_string(root->same_level_node);
    }
}

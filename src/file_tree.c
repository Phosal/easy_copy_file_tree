#include "../inc/file_tree.h"

void file_tree_init(file_tree_t *t)
{
    memset(t->file_tree_string, 0, MAX_FILE_STRING_LENGTH);
    t->print_column_flags = NULL;
    t->max_column = 0;
    t->root = NULL;
}

void file_tree_build(file_tree_t *file_tree,
                     tree_node_t *root,
                     WCHAR *search_path,
                     HANDLE handle_find,
                     WIN32_FIND_DATAW *find_data)
{
    bool if_first_folder = true;
    bool if_first_file = true;
    tree_node_t *folder_head = NULL;
    tree_node_t *folder_end = NULL;
    tree_node_t *file_head = NULL;
    tree_node_t *file_end = NULL;

    handle_find = FindFirstFileW(search_path, find_data);

    if (handle_find == INVALID_HANDLE_VALUE)
    {
        printf("Error：cannot open directory or cannot find any file in this directory\n");
        root->next_level_node = NULL;
        return;
    }

    do
    {
        if (wcscmp(find_data->cFileName, L".") == 0 ||
            wcscmp(find_data->cFileName, L"..") == 0 ||
            wcscmp(find_data->cFileName, L".git") == 0)
            continue;
        tree_node_t *new_node = create_new_node(find_data->cFileName, find_data->dwFileAttributes);
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

    tree_node_t *current_node = root->next_level_node;
    search_path[wcslen(search_path) - 1] = L'\0';
    while (current_node != NULL && current_node->node_type == IS_FOLDER)
    {
        WCHAR search_next_level_path[MAX_PATH];
        _snwprintf(search_next_level_path, sizeof(search_next_level_path), L"%s%s\\*", search_path, current_node->name);
        WIN32_FIND_DATAW find_next_level_data;
        HANDLE handle_next_level_find;
        file_tree_build(file_tree,
                        current_node,
                        search_next_level_path,
                        handle_next_level_find,
                        &find_next_level_data);
        current_node = current_node->same_level_node;
    }
}

void file_tree_mark(file_tree_t *file_tree,
                    tree_node_t *root,
                    int current_column,
                    bool if_my_dad_last_child)
{
    static int current_layer = 1;
    if (current_column > file_tree->max_column)
    {
        file_tree->max_column = current_column;
    }
    root->is_my_dad_the_last_child = if_my_dad_last_child;
    root->am_i_the_last_child = (root->same_level_node == NULL) ? true : false;
    root->layer = current_layer++;
    root->column = current_column;
    if (root->next_level_node != NULL)
    {
        file_tree_mark(file_tree, root->next_level_node, current_column + 1, root->am_i_the_last_child);
    }
    if (root->same_level_node != NULL)
    {
        file_tree_mark(file_tree, root->same_level_node, current_column, root->is_my_dad_the_last_child);
    }
}

void file_tree_string_make(file_tree_t *file_tree,
                           tree_node_t *root)
{
    if (root->is_my_dad_the_last_child == true)
    {
        if (root->column - 2 > 0)
        {
            file_tree->print_column_flags[root->column - 2] = false;
        }
    }
    for (int column = 1; column < root->column; column++)
    {
        if (column != root->column - 1)
        {
            if (file_tree->print_column_flags[column] == true)
            {
                wcscat(file_tree->file_tree_string, L"│");
                wcscat(file_tree->file_tree_string, L"  ");
            }
            else
            {
                wcscat(file_tree->file_tree_string, L" ");
                wcscat(file_tree->file_tree_string, L"  ");
            }
        }
        else
        {
            if (root->am_i_the_last_child == true)
            {
                wcscat(file_tree->file_tree_string, L"└");
                wcscat(file_tree->file_tree_string, L"──");
            }
            else
            {
                wcscat(file_tree->file_tree_string, L"├");
                wcscat(file_tree->file_tree_string, L"──");
            }
        }
    }
    wcscat(file_tree->file_tree_string, root->name);
    if (root->node_type == IS_FOLDER)
    {
        wcscat(file_tree->file_tree_string, L"/\n");
    }
    else
    {
        wcscat(file_tree->file_tree_string, L"\n");
    }

    if (root->next_level_node != NULL)
    {
        file_tree_string_make(file_tree, root->next_level_node);
    }
    if (root->same_level_node != NULL)
    {
        file_tree_string_make(file_tree, root->same_level_node);
    }
}
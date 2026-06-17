#ifndef __FILE_TREE_H__
#define __FILE_TREE_H__

#include "tree.h"
#include "windows.h"

#define MAX_FILE_STRING_LENGTH 100000

typedef struct
{
    tree_node_t *root;
    WCHAR file_tree_string[MAX_FILE_STRING_LENGTH];
    bool *print_column_flags;
    int max_column;
} file_tree_t;

/**
 * @brief 初始化文件树的结构体
 * @param file_tree 将要搜索的文件夹下文件树的结构体指针
 * @retval none
 */
void file_tree_init(file_tree_t *file_tree);

/**
 * @brief 递归搜索指定文件夹下所有文件夹和文件，根据文件树结构构造数据结构
 *
 */
void file_tree_build(file_tree_t *file_tree,
                     tree_node_t *root,
                     WCHAR *search_path,
                     HANDLE handle_find,
                     WIN32_FIND_DATAW *find_data);

void file_tree_mark(file_tree_t *file_tree,
                    tree_node_t *root,
                    int current_column,
                    bool if_my_dad_last_child);

void file_tree_string_make(file_tree_t *file_tree,
                           tree_node_t *root);

#endif
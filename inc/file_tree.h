#ifndef FILE_TREE_H
#define FILE_TREE_H

#include <stdbool.h>
#include <windows.h>
#include "my_error.h"

#define MAX_FILE_STRING_LENGTH 100000

typedef enum
{
    IS_FOLDER = 0,
    IS_FILE,
    IS_NOT_KNOWN
} node_type_t;

typedef struct tree_node tree_node_t;
typedef struct file_tree file_tree_t;

struct tree_node
{
    WCHAR *name;
    node_type_t node_type;
    struct tree_node *next_level_node;
    struct tree_node *same_level_node;
    int layer;
    int column;
    bool am_i_the_last_child;
    bool is_my_dad_the_last_child;
};

struct file_tree
{
    tree_node_t *root;
    WCHAR file_tree_string[MAX_FILE_STRING_LENGTH];
    bool *print_column_flags;
    int max_column;
};

/**
 * @brief 创建一个新的tree_node_t类型结构体，创建动态内存
 * @param node_name 应为WCHAR *类型，指向字符串的指针。该函数会将字符串赋值给赋值给新节点的名字
 * @param file_attributes 应为winnt.h文件中FILE_ATTRIBUTE_开头的宏定义。该函数会根据该值判断新节点代表文件夹/文件
 * @retval 创建的结构体的指针
 */

err_t create_new_node(WCHAR *node_name, DWORD file_attributes, tree_node_t **new_node);

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
err_t file_tree_build(file_tree_t *file_tree,
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
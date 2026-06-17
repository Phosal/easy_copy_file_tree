#ifndef __LINK_LIST_H__
#define __LINK_LIST_H__

#include <stdio.h>
#include <stdbool.h>
#include <windows.h>

typedef enum
{
    IS_FOLDER = 0,
    IS_FILE,
    IS_NOT_KNOWN
} node_type_t;

typedef struct tree_node tree_node_t;

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

/**
 * @brief 创建一个新的tree_node_t类型结构体，创建动态内存
 * @param node_name 应为WCHAR *类型，指向字符串的指针。该函数会将字符串赋值给赋值给新节点的名字
 * @param file_attributes 应为winnt.h文件中FILE_ATTRIBUTE_开头的宏定义。该函数会根据该值判断新节点代表文件夹/文件
 * @retval 创建的结构体的指针
 */

tree_node_t *create_new_node(WCHAR *node_name, DWORD file_attributes);

#endif
#include <stdio.h>
#include <stdlib.h>
#include "../inc/tree.h"

tree_node_t *create_new_node(WCHAR *node_name, DWORD file_attributes)
{
    tree_node_t *new_node = (tree_node_t *)malloc(sizeof(tree_node_t));
    new_node->name = (WCHAR *)malloc(sizeof(WCHAR) * (wcslen(node_name) + 1));
    wcscpy(new_node->name, node_name);
    if (file_attributes & FILE_ATTRIBUTE_DIRECTORY)
    {
        new_node->node_type = IS_FOLDER;
    }
    else
    {
        new_node->node_type = IS_FILE;
    }
    new_node->next_level_node = NULL;
    new_node->same_level_node = NULL;
    return new_node;
}

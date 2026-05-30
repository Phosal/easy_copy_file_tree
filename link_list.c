#include <stdio.h>
#include <stdlib.h>
#include "link_list.h"

list_node_t *create_new_node(char *node_name, DWORD file_attributes)
{
    list_node_t *new_node=(list_node_t *)malloc(sizeof(list_node_t));
    new_node->name=(char *)malloc(sizeof(char)*(strlen(node_name)+1));
    strcpy(new_node->name,node_name);
    if (file_attributes & FILE_ATTRIBUTE_DIRECTORY)
    {
        new_node->node_type=IS_FOLDER;
    }
    else
    {
        new_node->node_type = IS_FILE;
    }
    new_node->next_level_node=NULL;
    new_node->same_level_node=NULL;
    return new_node;
}
#ifndef LINK_LIST_H
#define LINK_LIST_H

#include <stdbool.h>
#include <windows.h>

typedef enum
{
    IS_FOLDER=0,
    IS_FILE,
    IS_NOT_KNOWN
}node_type_t;

typedef struct list_node list_node_t;

struct list_node
{
    char *name;
    node_type_t node_type;
    struct list_node *next_level_node;
    struct list_node *same_level_node;
    int layer;
    int column;
    bool am_i_the_last_child;
    bool is_my_dad_the_last_child;
};

list_node_t *create_new_node(char *node_name, DWORD file_attributes);

#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <stdbool.h>
#include "link_list.h"

#define MAX_PATH_LENGTH 260

char *path;
char final_string[1000]={'\0'};
int max_column=0;
bool *if_print_column=NULL;

void build_list(list_node_t *root, 
                char *search_path,
                HANDLE handle_find, 
                WIN32_FIND_DATA *find_data);

void make_mark(list_node_t *root, int current_column, bool if_my_dad_last_child);

void make_string(list_node_t *root);

int main(int argc, char *argv[])//目录最终目标必须是文件夹
{
    SetConsoleOutputCP(65001);
    char search_path[MAX_PATH_LENGTH];
    WIN32_FIND_DATA find_data;
    HANDLE handle_find;
    if(!OpenClipboard(NULL))
    {
        printf("剪切板打开失败\n");
        return 0;
    }

    if(!EmptyClipboard())
    {
        printf("剪切板清空失败\n");
        CloseClipboard();
        return 0;
    }

    path=(argc>1) ? argv[1] : ".";
    snprintf(search_path,sizeof(search_path),"%s\\*",path);
    
    list_node_t *root=NULL;
    for (int i = strlen(path); i > 0; i--)
    {
        if(path[i]=='\\')
        {
            root = create_new_node(&path[i + 1], FILE_ATTRIBUTE_DIRECTORY);
            break;
        }
    }
    build_list(root,
               search_path,
               handle_find,
               &find_data);
    
    make_mark(root,1,true);
    if_print_column=(bool *)malloc(sizeof(bool)*max_column);
    for(int i=0;i<max_column;i++)
    {
        if_print_column[i]=true;
    }
    make_string(root);

    size_t len = strlen(final_string)+1;
    HANDLE hGlob = GlobalAlloc(GMEM_MOVEABLE,len);
    if(hGlob == NULL)
    {
        printf("内存分配失败\n");
        CloseClipboard();
        return 0;
    }

    char *pBuf = (char *)GlobalLock(hGlob);
    if(pBuf==NULL)
    {
        GlobalFree(hGlob);
        CloseClipboard();
        return 0;
    }

    memcpy(pBuf,final_string,len);
    GlobalUnlock(hGlob);

    if(SetClipboardData(CF_TEXT,hGlob)==NULL)
    {
        printf("设置剪切板数据失败\n");
        CloseClipboard();
        GlobalFree(hGlob);
        return 0;
    }
    CloseClipboard();
    printf("复制成功：\n%s\n",final_string);
    //printf("%s\n",final_string);
    system("pause");
    return 0;
}

void build_list(list_node_t *root, 
                char *search_path,
                HANDLE handle_find, 
                WIN32_FIND_DATA *find_data)
{
    bool if_first_folder=true;
    bool if_first_file=true;
    list_node_t *folder_head = NULL;
    list_node_t *folder_end = NULL;
    list_node_t *file_head = NULL;
    list_node_t *file_end = NULL;

    handle_find = FindFirstFile(search_path, find_data);

    if (handle_find == INVALID_HANDLE_VALUE)
    {
        fprintf(stderr, "Error：cannot open directory \"%s\" or cannot find any file in this directory\n", path);
        root->next_level_node=NULL;
        return;
    }

    do
    {
        if(strcmp(find_data->cFileName,".")==0||
           strcmp(find_data->cFileName,"..")==0)
           continue;
        list_node_t *new_node=create_new_node(find_data->cFileName,find_data->dwFileAttributes);
        if(new_node->node_type==IS_FOLDER)
        {
            if(if_first_folder)
            {
                folder_head=new_node;
                folder_end=new_node;
                if_first_folder=false;
            }
            else
            {
                new_node->same_level_node=folder_end->same_level_node;
                folder_end->same_level_node=new_node;
                folder_end=new_node;
            }
        }
        else
        {
            if(if_first_file)
            {
                file_head=new_node;
                file_end=new_node;
                if_first_file=false;
            }
            else
            {
                new_node->same_level_node=file_end->same_level_node;
                file_end->same_level_node=new_node;
                file_end=new_node;
            }
        }
    } while (FindNextFile(handle_find, find_data));
    //进入此if证明root文件夹内没有任何文件(.和..文件夹已被过滤)
    if(if_first_folder==true&&if_first_file==true)
    {
        root->next_level_node=NULL;
        return;
    }
    //如果进入下面的if，证明至少有一个文件夹或者至少有一个文件
    if (if_first_folder == true)//只有文件
    {
        root->next_level_node=file_head;
        return;
    }
    //如果运行到这一步，证明只有文件夹或者有文件夹也有文件
    folder_end->same_level_node=file_head;
    root->next_level_node=folder_head;
    
    list_node_t *current_node=root->next_level_node;
    search_path[strlen(search_path) - 1] = '\0';
    while(current_node!=NULL && current_node->node_type==IS_FOLDER)
    {
        char search_next_level_path[MAX_PATH_LENGTH];
        snprintf(search_next_level_path,sizeof(search_next_level_path),"%s%s\\*",search_path,current_node->name);
        WIN32_FIND_DATA find_next_level_data;
        HANDLE handle_next_level_find;
        build_list(current_node, search_next_level_path, handle_next_level_find, &find_next_level_data);
        current_node = current_node->same_level_node;
    }
}

void make_mark(list_node_t *root,int current_column,bool if_my_dad_last_child)
{
    static int current_layer=1;
    if(current_column>max_column)
    {
        max_column=current_column;
    }
    root->is_my_dad_the_last_child=if_my_dad_last_child;
    root->am_i_the_last_child=(root->same_level_node==NULL)?true:false;
    root->layer=current_layer++;
    root->column=current_column;
    if(root->next_level_node!=NULL)
    {
        make_mark(root->next_level_node,current_column+1,root->am_i_the_last_child);
    }
    if(root->same_level_node!=NULL)
    {
        make_mark(root->same_level_node,current_column,root->is_my_dad_the_last_child);
    }

}

void make_string(list_node_t *root)
{
    if(root->is_my_dad_the_last_child==true)
    {
        if(root->column-2>0)
        {
            if_print_column[root->column-2]=false;
        }
    }
    for(int column=1;column<root->column;column++)
    {
        if(column!=root->column-1)
        {
            if(if_print_column[column]==true)
            {
                strcat(final_string, "│");
                strcat(final_string, "  ");
                //printf("│");
                //printf("  ");
            }
            else
            {
                strcat(final_string, " ");
                strcat(final_string, "  ");
                //printf(" ");
                //printf("  ");
            }
        }
        else
        {
            if(root->am_i_the_last_child==true)
            {
                strcat(final_string, "└");
                strcat(final_string, "──");
                //printf("└");
                //printf("──");
            }
            else
            {
                strcat(final_string, "├");
                strcat(final_string, "──");
                //printf("├");
                //printf("──");
            }
        }
    }
    strcat(final_string,root->name);
    //printf("%s",root->name);
    if(root->node_type==IS_FOLDER)
    {
        strcat(final_string, "/\n");
        //printf("/\n");
    }
    else
    {
        strcat(final_string, "\n");
        //printf("\n");
    }

    if(root->next_level_node!=NULL)
    {
        make_string(root->next_level_node);
    }
    if(root->same_level_node!=NULL)
    {
        make_string(root->same_level_node);
    }
}

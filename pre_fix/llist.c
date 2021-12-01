#include<string.h>
#include<stdlib.h>
#include"llist.h"


jrb_type *list_make()
{
    jrb_type *p;
    p = (jrb_type*)malloc(sizeof(jrb_type));
    memset(p->eng, 0, 100);
    p->next = NULL;
    return p;
}


jrb_type *list_insert(jrb_type **cur, char *str)
{
    jrb_type *p;
    if (cur == NULL)
    {
        *cur = list_make();
        p = *cur;
    }
    //while(cur->next != NULL)cur = cur->next;
    (*cur)->next = list_make();
    strcpy((*cur)->next->eng, str);
    (*cur) = (*cur)->next;
    return p;
}


void list_free(jrb_type **root)
{
    jrb_type *p = *root;
    while(p != NULL)
    {
        *root = (*root)->next;
        free(p);
        p = (*root);
    }
}

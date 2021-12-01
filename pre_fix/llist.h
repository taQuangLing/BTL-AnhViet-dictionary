#ifndef _LLIST_
#define _LLIST_

typedef struct jrb_t
{
    char eng[200];
    struct jrb_t *next;
}jrb_type;


jrb_type *list_make();

jrb_type *list_insert(jrb_type **cur, char *str);

void list_free(jrb_type **root);

#define list_traverse(ptr, lst) \
  for(ptr = lst; ptr != NULL; ptr = ptr->next)
#endif

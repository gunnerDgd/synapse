#include <stdlib.h>
#include <stdio.h>

typedef struct list_block
{
    void*  block_context;
    size_t block_size   ;

    struct list_block* block_next,
                     * block_prev;
}   list_block;

typedef struct list_context
{
    list_block* list_start,
              * list_end  ;

    uint64_t    list_count;
}   list_context;

list_context* list_create();
void          list_delete(list_context* del_list);

void          list_insert(list_context* ins_ent, size_t ins_index, void* ins_ctx, size_t ins_size);
void          list_delete(list_context* del_ent, size_t del_index);

void*         list_get      (list_context* get_ent, size_t get_index);
list_block*   list_get_block(list_context* get_ent, size_t get_index);

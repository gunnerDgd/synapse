#include <synapse/c_container/list/list.h>

list_context* list_create()                      { return (list_context*)malloc(sizeof(list_context)); }
void          list_delete(list_context* del_list)
{
    list_block* del_ptr = del_list->list_end;
    while      (del_ptr != del_list->list_start) {
        free     (del_ptr->block_context);
        del_ptr = del_ptr->list_prev;

        free     (del_ptr->list_next);
    }
}

void  list_insert(list_context* ins_ent, size_t ins_index, void* ins_ctx, size_t ins_size)
{
    list_block* ins_ptr = list_get_block(ins_ent, ins_index);
              * ins_new = malloc        (sizeof(list_block));

    ins_new->block_context = ins_ctx ;
    ins_new->block_size    = ins_size;

    if(!ins_ptr->block_next) // Last block of the list.
        

    ins_ptr->block_next    = ins_new;
    ins_new->block_prev    = ins_ptr;
}

void  list_delete(list_context* del_ent, size_t del_index)
{
    list_block* del_ptr = list_get_block(del_ent, del_index);
    del_ptr->list_prev->
}

list_block* list_get_block(list_context* get_ent, size_t get_index)
{
    list_block* get_ptr = get_ent->list_start;
    if         (get_ent->list_size == 0) return NULL;

    while      (get_index-- > 0 && !get_ptr->block_next)
        get_ptr = get_ptr->block_next;

    return get_ptr;
}

void* list_get   (list_context* get_ent, size_t get_index)
{
    list_block* get_ptr = list_get_block(get_end, get_index);
    return    (!get_ptr) ? NULL : get_ptr->block_context;
}


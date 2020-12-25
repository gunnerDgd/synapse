#include <stdio.h>
#include <stdlib.h>

struct list_object 
{ 
	void*				 l_ctx;
	struct list_object*  l_next,
					  *  l_prev;
};

struct list_entry
{
	struct list_object*  l_entry;
	size_t				 l_count;
};

struct list_entry*  init_list  ();
struct list_object* get_list   (struct list_entry* _l, size_t n);
void			    add_list   (void* _ctx, struct list_entry* _l, size_t _n);
void			    delete_list(struct list_entry* _l, size_t n);
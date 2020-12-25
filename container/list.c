#include "list.h"

struct list_entry* init_list()
{
	struct list_entry* _entry = malloc(sizeof(struct list_entry));
	
	_entry->l_count = 0;
	_entry->l_entry = NULL;

	return _entry;
}

struct list_object* get_list(struct list_entry* _l, size_t n)
{
	struct list_object* _i = _l->l_entry;
	if (n > _l->l_count) n = _l->l_count;

	while (n-- > 0)
	{
		if (_i->l_next != NULL && _i != NULL)
			_i = _i->l_next;
		else
			break;
	}

	return _i;
}

void			  add_list(void* _ctx, struct list_entry* _l, size_t _n)
{
	struct list_object* _i = get_list(_l, _n);
	if (_i == NULL)
	{
		struct list_object* _new = malloc(sizeof(struct list_object));
		_new->l_ctx  = _ctx;
		_new->l_next = NULL;
		_new->l_prev = NULL;

		_l  ->l_entry = _i;
	}
	else if (!_i->l_next)
	{
		struct list_object* _new = malloc(sizeof(struct list_object));
		
		_new->l_ctx  = _ctx;
		_new->l_prev = _i;
		_i->l_next   = _new;
	}
	else
	{
		struct list_object* _new = malloc(sizeof(struct list_object));
		
		_new->l_ctx = _ctx;
		
		_i->l_next->l_prev = _new;
		_i->l_next		   = _new;
	}

	_l->l_count++;
}

void			  delete_list(struct list_entry* _l, size_t n)
{
	struct list_object* _d = get_list(_l, n);
	if (_d == NULL)			 return;

	_d->l_prev->l_next = _d->l_next;
	_d->l_next->l_prev = _d->l_prev;

	free(_d);
	_l->l_count--;
}
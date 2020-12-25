#include "queue.h"

struct queue_entry*  init_queue()
{
	struct queue_entry* _entry = malloc(sizeof(struct queue_entry));
	
	_entry->q_start = NULL;
	_entry->q_end   = NULL;
	_entry->q_size  = 0;

	return _entry;
}

void				 enqueue(struct queue_entry* _entry, void* _ctx)
{
	struct queue_object* _obj = malloc(sizeof(struct queue_object*));;
	_obj->q_ctx				  = _ctx;

	if (_entry->q_end == NULL) // First to Start
	{
		_entry->q_start = _obj;
		_entry->q_end   = _obj; return;
	}
	else
		_entry->q_end->q_next = _obj;
}

struct queue_object* dequeue(struct queue_entry* _entry)
{
	return _entry->q_start;
}
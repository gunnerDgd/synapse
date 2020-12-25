#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct queue_object { void* q_ctx; struct queue_object* q_next; };
struct queue_entry  
{ 
	struct queue_object* q_start,
					   * q_end;
	size_t q_size; 
};

struct queue_entry*  init_queue();
void				 enqueue   (struct queue_entry* _entry, void* _ctx);
struct queue_object* dequeue   (struct queue_entry* _entry);
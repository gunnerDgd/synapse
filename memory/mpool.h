#pragma once
#include <synapse/container/queue.h>
#include <Windows.h>

struct mpool
{
	struct queue_entry* mpool_entry;

	union {
		HANDLE			   mpool_page_handle;
		void*			   mpool_heap_base_pointer;
	};
};

void* mpool_acquire(struct mpool* _pool);
void  mpool_release(struct mpool* _pool, void* _pool_ptr);

struct mpool* init_heap_mpool(size_t pool_msize, size_t pool_mlength);
struct mpool* init_page_mpool(size_t pool_msize, size_t pool_mlength);
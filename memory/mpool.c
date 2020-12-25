#include "mpool.h"

struct mpool* init_heap_mpool(size_t pool_msize, size_t pool_mlength)
{
	struct mpool* _pool = malloc(sizeof(struct mpool));
	size_t		  _pool_size = pool_msize * pool_mlength;

	_pool->mpool_entry			   = init_queue();
	_pool->mpool_heap_base_pointer = malloc(pool_msize * pool_mlength);

	for (size_t i = 0; i < pool_mlength; i++)
		mpool_release(_pool, (uint8_t*)_pool->mpool_heap_base_pointer + i*pool_msize);

	return _pool;
}

struct mpool* init_page_mpool(size_t pool_msize, size_t pool_mlength)
{
	struct mpool* _pool		 = malloc(sizeof(struct mpool));
	size_t		  _pool_size = pool_msize * pool_mlength;
	
	_pool->mpool_entry		 = init_queue();
	_pool->mpool_page_handle = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE,
												(_pool_size >> 32), (_pool_size & 0xFFFFFFFF), NULL);

	void*  mpool_page_base   = MapViewOfFile(_pool->mpool_page_handle, FILE_MAP_ALL_ACCESS, 
											 0, 0, _pool_size);

	for (size_t i = 0; i < pool_mlength; i++)
		mpool_release(_pool, (uint8_t*)mpool_page_base + i*pool_msize);

	return _pool;
}

void* mpool_acquire(struct mpool* _pool)
{
	return dequeue(_pool->mpool_entry);
}

void  mpool_release(struct mpool* _pool, void* _pool_ptr)
{
	enqueue(_pool->mpool_entry, _pool_ptr);
}
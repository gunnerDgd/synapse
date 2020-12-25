#pragma once
#include <iostream>

extern "C" 
{ 
	#include <synapse/memory/mpool.h> 
}

namespace memory
{
	enum		mpool_location { heap, page };

	template <typename T, size_t N>
	class memory_pool
	{
	public:	
		memory_pool(mpool_location _loc);

		T*   acquire()		  { return (T*)mpool_acquire(mpool_ctx); }
		void release(T* _ptr) { mpool_release(mpool_ctx, (void*)_ptr); }

	private:
		struct mpool* mpool_ctx;
	};

	template <typename T, size_t N>
	memory::memory_pool<T, N>::memory_pool(mpool_location _loc)
	{
		if (_loc == mpool_location::heap)
			mpool_ctx = init_heap_mpool(sizeof(T), N);
		else
			mpool_ctx = init_page_mpool(sizeof(T), N);
	}
}
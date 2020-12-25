#pragma once
#include <synapse/memory/page.hpp>
#include <synapse/memory/heap.hpp>
#include <synapse/include/stream.hpp>
#include <synapse/sync/fence.hpp>

#include <thread>
#include <vector>

#define  MEMORY_POOL_SIZE 4096

namespace disk
{
	struct mstream_vector
	{
		mstream_vector()  { mstream_ctx = new memory::page_object<uint8_t>(4096); }
		~mstream_vector() { delete mstream_ctx; }

		memory::page_object<uint8_t> *mstream_ctx;
		synchronous::fence			  mstream_fence;

		bool						  mstream_dirty = false;
	};

	class  memory_stream : public stream::stream
	{
	public:
		memory_stream ();

		size_t  read  (uint8_t* r_ctx, size_t r_size);
		size_t  write (uint8_t* w_ctx, size_t w_size);
		
		void    offset(size_t   _off);

	private:
		std::vector<mstream_vector> mstream_vec;
		std::thread*				mstream_io_thread;
		
		stream*					    mstream_fstream;
		size_t						mstream_offset;
	};
}
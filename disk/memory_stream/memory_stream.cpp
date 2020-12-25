#include "memory_stream.hpp"

size_t disk::memory_stream::read  (uint8_t* r_ctx, size_t r_size)
{
	size_t r_cur_page = ((mstream_offset % 4096) == 0 && mstream_offset != 0)
					  ? (mstream_offset / 4096) : (mstream_offset / 4096 + 1);

	size_t r_cur_off = mstream_offset % 4096;
	while (r_size > 0)
	{

	}

}

size_t disk::memory_stream::write (uint8_t* w_ctx, size_t w_size)
{
	
}

void   disk::memory_stream::offset(size_t   _off)
{

}
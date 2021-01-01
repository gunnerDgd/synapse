#include "memory_stream.hpp"

size_t stream::memory_stream::read  (uint8_t* _rbuf, size_t _rsize)
{
    size_t block_read = 0, 
           read_size  = 0;
    size_t block_read_size = 0

    if(memory_stream_tied == nullptr) return 0;

    while(_rsize >= 0)
    {
        block_read      = memory_stream_offset / stream_block_size;
        block_read_size = stream_block_size - (memory_stream_offset % stream_block_size);
        
        if(memory_stream_block.find(block_read) == memory_stream_block.end())
            memory_stream_tied->read(_rbuf + read_size, block_read_size);
        else
            memcpy(_rbuf + read_size, 
                   memory_stream_block[block_read].block_context + (memory_stream_offset % stream_block_size),
                   block_read_size);

        _rsize -= block_read_size;
    }
}

size_t stream::memory_stream::write (uint8_t* _wbuf, size_t _wsize)
{

}
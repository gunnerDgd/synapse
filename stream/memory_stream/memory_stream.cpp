#include "memory_stream.hpp"

size_t stream::memory_stream::read  (uint8_t* _rbuf, size_t _rsize)
{
    size_t block_id     = 0,
           block_offset = 0,
           block_read   = 0;

    while(block_read < _rsize)
    {
        block_id          = memory_stream_offset / stream_block_size;
        block_offset      = memory_stream_offset % stream_block_size;

        if(memory_stream_block.find(block_id) == memory_stream_block.end())
            memory_stream_connected->read(_rbuf + block_read,
                                          stream_block_size - block_offset);
        else
        {
            
        }

        block_read           += stream_block_size - block_offset;
        memory_stream_offset += stream_block_size - block_offset;
    }
}

size_t stream::memory_stream::write (uint8_t* _wbuf, size_t _wsize)
{

}
#include <iostream>

#include <thread>
#include <mutex>

#include <atomic>
#include <map>

#include <synapse/memory/memory_object/type/page.hpp>
#include <synapse/stream/stream.hpp>

#define stream_block_size 4096

#define stream_block_id(_offset, _bsize) (_offset / _bsize)
#define stream_block_offset(_offset, _bsize) (_offset % _bszie)

namespace stream
{
    class memory_stream
    {
    public:
        void connect   (stream::stream* _tstream) { memory_stream_tied = _tstream; }
        void disconnect()                         { memory_stream_tied = nullptr; }

    public:
        size_t read  (uint8_t* _rbuf, size_t _rsize);
        size_t write (uint8_t* _wbuf, size_t _wsize);

        void   offset(size_t _offset) { memory_stream_offset = _offset; }

    private:
        stream::stream*                memory_stream_connected   = nullptr;

    private:
        uint8_t* memory_stream_block  = new uint8_t[4096 * 1024];
        size_t   memory_stream_offset = 0, 
                 memory_block_start   = memory_stream_offset;
    }
}
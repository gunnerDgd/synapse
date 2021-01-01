#include <iostream>

#include <thread>
#include <mutex>

#include <atomic>
#include <map>

#include <synapse/memory/memory_object/page_memory.hpp>
#include <synapse/stream/stream.hpp>

#define stream_block_size 4096

#define stream_block_id(_offset, _bsize) (_offset / _bsize)
#define stream_block_offset(_offset, _bsize) (_offset % _bszie)

namespace stream
{
    struct stream_block
    {
        std::atomic<bool> block_dirty;
        uint8_t           block_context[stream_block_size];

        std::mutex        block_lock;
    };

    class memory_stream
    {
    public:
        void tie_stream  (stream::stream* _tstream) { memory_stream_tied = _tstream; }
        void untie_stream()                         { memory_stream_tied = nullptr; }

    public:
        size_t read  (uint8_t* _rbuf, size_t _rsize);
        size_t write (uint8_t* _wbuf, size_t _wsize);

        void   offset(size_t _offset) { memory_stream_offset = _offset; }

    private:
        stream::stream*  memory_stream_tied   = nullptr;
        size_t           memory_stream_offset = 0;

    private:
        std::map<size_t, stream_block> memory_stream_block;
    }
}
#include <iostream>

namespace memory
{
    struct stream_block { uint8_t block_context[4096]; };
    class  stream_block_list
    {
    public:
        stream_block_list();

    private:
        size_t       block_start_offset = 0;
        stream_block block_pool[1024];
    };
}
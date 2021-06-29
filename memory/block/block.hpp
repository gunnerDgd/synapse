#include <synapse/memory/memory.hpp>

namespace synapse {
namespace memory  {

    class block
    {
    public:
        block(synapse::memory::memory& bl_mem, 
              size_t                   bl_off  = 0,
              size_t                   bl_size = bl_mem.memory_size());
        
        void*  memory_pointer() { return block_pointer; }
        size_t memory_size   () { return block_size   ; } 

    protected:
        void*  block_pointer;
        size_t block_size   ;
    }

}
}

synapse::memory::block::block(synapse::memory::memory& bl_mem , 
                              size_t                   bl_off ,
                              size_t                   bl_size)
{
    if(bl_off + bl_size > block_pointer.memory_size())
        return; // Wrong Range Of Memory.

    block_pointer = (uint8_t*)bl_mem.memory_pointer() + bl_off;
    block_size    = bl_size;
}
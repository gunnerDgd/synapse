#pragma once

#include <iostream>
#include <synapse/memory/memory_attr.hpp>

namespace synapse {
namespace memory  {
    
    class memory
    {
    public:
        memory(void*  mem_addr = nullptr, 
               size_t mem_size = 0)
            : memory_block_size(mem_size),
              memory_address   (mem_addr),
            
              memory_lock_type (synapse::memory::lock_type::unlocked),
              memory_state_flag(synapse::memory::memory_state::normal) { }

        void*  memory_pointer() { return memory_address   ; }
        size_t memory_size   () { return memory_block_size; }

        synapse::memory::memory_state current_state()                                 { return memory_state_flag; }
        void                          set_state    (synapse::memory::memory_state& s) { memory_state_flag = s; }

    protected:
        synapse::memory::memory_state memory_state_flag;
        size_t                        memory_block_size;
        void*                         memory_address   ;
    };
}
}
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

    public:
        bool                          lock  ();
        bool                          unlock();

        synapse::memory::memory_state current_state()                                 { return memory_state_flag; }
        void                          set_state    (synapse::memory::memory_state& s) { memory_state_flag = s; }

    protected:
        synapse::memory::memory_state memory_state_flag;
        size_t                        memory_block_size;
        void*                         memory_address   ;

    protected:
        synapse::memory::lock_type    memory_lock_type ;
    };
}
}

bool synapse::memory::memory::lock  ()
{
    int l_res        = mlock(memory_address, memory_block_size);
    memory_lock_type = (l_res >= 0) ? synapse::memory::lock_type::locked
                                    : synapse::memory::lock_type::unlocked;

    return (l_res >= 0) ? true : false;
}

bool synapse::memory::memory::unlock()
{
    if(memory_lock_type == synapse::memory::lock_type::unlocked)
        return true;

    int ul_res       = munlock(memory_address, memory_block_size);
    memory_lock_type = (ul_res >= 0) ? synapse::memory::lock_type::unlocked
                                     : synapse::memory::lock_type::locked;

    return (ul_res >= 0) ? true : false;
}
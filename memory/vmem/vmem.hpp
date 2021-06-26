#pragma once

#include <synapse/memory/memory.hpp>
#include <synapse/memory/memory_attr.hpp>

#include <variant>

namespace synapse {
namespace memory  {
    
    class vmem : public synapse::memory::memory
    {
    public:
        vmem (size_t vm_size,
              int    vm_protect = protect_type::read | protect_type::write,
              void*  vm_addr    = nullptr);

        vmem(synapse::memory::vmem& copy_vmem)
        { 
            memory_block_size      = copy_vmem.memory_block_size     ;
            memory_state_flag      = copy_vmem.memory_state_flag     ;
            memory_address         = copy_vmem.memory_address        ;
            memory_reference_count = copy_vmem.memory_reference_count;
            
            ++(*memory_reference_count); 
        }

        vmem(synapse::memory::vmem&& copy_vmem)
        { 
            memory_block_size        = copy_vmem.memory_block_size     ;
            memory_state_flag        = copy_vmem.memory_state_flag     ;
            memory_address           = copy_vmem.memory_address        ;
            memory_reference_count   = copy_vmem.memory_reference_count;
            
            copy_vmem.memory_address = nullptr;
        }

        ~vmem();
    };
}
}

synapse::memory::vmem::vmem(size_t vm_size    ,
                            int    vm_protect ,
                            void*  vm_addr    )
    : memory(nullptr, vm_size)
{
    memory_address = mmap(vm_addr                    , 
                          vm_size                    ,
                          vm_protect                 ,
                          MAP_PRIVATE | MAP_ANONYMOUS,
                          -1                         ,
                          0
                         );

    memory_reference_count  = new uint64_t;
    *memory_reference_count = 1;

    if(!memory_address) // If Memory Allocation Failed.
        memory_state_flag = synapse::memory::memory_state::allocate_error;    
}

synapse::memory::vmem::~vmem()
{
    if(*memory_reference_count > 1)
        return;

    int um_res = munmap(memory_address, memory_block_size);
    if (um_res < 0 && memory_state_flag == synapse::memory::memory_state::normal)
    {
        std::cerr << "FATAL ## MUNMAP Failed...\n";
        exit(1);
    }

    std::cout << "Memory Unallocated\n";
}
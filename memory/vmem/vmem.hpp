#include <synapse/memory/memory.hpp>
#include <synapse/memory/memory_attr.hpp>

namespace synapse
{
namespace memory
{
    class vmem : public synapse::memory::memory
    {
    public:
        vmem (size_t vm_size,
              int    vm_protect = protect_type::read | protect_type::write,
              void*  vm_addr    = nullptr);

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

    if(!memory_address) // If Memory Allocation Failed.
        memory_state_flag = synapse::memory::memory_state::allocate_error;    
}

synapse::memory::vmem::~vmem()
{
    if(memory_lock_type == synapse::memory::lock_type::locked)
        this->unlock();
    
    int um_res = munmap(memory_address, memory_block_size);
    if (um_res < 0 && memory_state_flag == synapse::memory::memory_state::normal)
    {
        std::cerr << "FATAL ## MUNMAP Failed...\n";
        exit(1);
    }
}
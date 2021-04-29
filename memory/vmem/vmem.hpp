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
              int    vm_protect = (int)protect_type::read | (int)protect_type::write,
              void*  vm_addr    = nullptr);

        ~vmem();
        
    public:
        synapse::memory::memory_state state() { return vmem_state; }

    protected:
        synapse::memory::memory_state vmem_state;
    };
}
}

synapse::memory::vmem::vmem(size_t vm_size    ,
                            int    vm_protect ,
                            void*  vm_addr    )
{
    memory_address = mmap(vm_addr                    , 
                          vm_size                    ,
                          vm_protect                 ,
                          MAP_PRIVATE | MAP_ANONYMOUS,
                          -1                         ,
                          0
                         );
    
    if(!memory_address)
        vmem_state = synapse::memory::memory_state::allocate_error;    
}

synapse::memory::vmem::~vmem()
{
    int um_res = munmap(memory_address, memory_size);
    if (um_res < 0 && vmem_state == synapse::memory::memory_state::normal)
    {
        std::cerr << "FATAL ## MUNMAP Failed...\n";
        exit(1);
    }
}
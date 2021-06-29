#include <synapse/memory/vmem/vmem.hpp>
#include <synapse/memory/block/block.hpp>

int main()
{
    synapse::memory::vmem  tst_vmem;
    synapse::memory::block tst_block(tst_vmem, 2, 1024);
    
    std::cout << tst_vmem.memory_pointer() << std::endl << tst_block.memory_pointer() << std::endl;
}
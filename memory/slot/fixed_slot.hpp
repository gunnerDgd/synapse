#include <synapse/memory/vmem/vmem.hpp>
#include <synapse/memory/block/block.hpp>
#include <synapse/lockfree/stack/fixed_stack.hpp>

namespace synapse {
namespace memory  {

    template <uint32_t page_count, uint32_t slot_size>
    class fixed_slot: protected synapse::lockfree::fixed_stack<synapse::memory::block, (page_count * 4096 / slot_size)>,
                      protected synapse::memory::vmem
    {
    public:
        fixed_slot();

        synapse::memory::block& acquire()                               { return this->pop(); }
        void                    release(synapse::memory::block& bl_rel) { this->push(bl_rel); }
    };

}
}

template <uint32_t page_count, uint32_t slot_size>
synapse::memory::fixed_slot<page_count, slot_size>::fixed_slot()
    : synapse::memory::vmem(page_count)
{
    for(uint32_t bl_it = 0                      ;
        bl_it < (page_count * 4096 / slot_size) ; 
        bl_it++)                                this->push(synapse::memory::block(*this, bl_it * slot_size, slot_size));

}
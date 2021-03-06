#include <synapse/memory/slot/slot.hpp>
#include <synapse/debug_tool/runtime_timer.hpp>
#include <iostream>

int main()
{
    synapse::memory::slot test_slot(8192);

    {
        synapse::debug_tool::runtime_timer tmr;
        uint8_t* tst_heap = new uint8_t[8192];
    }

    {
        synapse::debug_tool::runtime_timer tmr;
        void* tst_heap  = test_slot.acquire().memory_pointer();
    }
}
#include <synapse/memory/slot/fixed_slot.hpp>
#include <synapse/debug_tool/runtime_timer.hpp>

int main()
{
    {
        synapse::memory::fixed_slot<20, 256> slot_test;
        synapse::debug_tool::runtime_timer   test_timer;

        slot_test.acquire();
    }

    {
        synapse::debug_tool::runtime_timer   test_timer;
        uint8_t* slot_test = new uint8_t[256];
    }
}
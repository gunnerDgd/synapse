#include <synapse/memory/vmem/vmem.hpp>
#include <synapse/debug_tool/runtime_timer.hpp>

int main()
{
    synapse::debug_tool::runtime_timer timer("## [DEBUG]");
    synapse::memory::vmem tst_1(4096);
    std::cout << tst_1.reference_count() << '\n';
    {
        synapse::memory::vmem tst_2(tst_1);
        std::cout << tst_2.reference_count() << '\n';
        std::cout << tst_1.reference_count() << '\n';
    }
    std::cout << tst_1.reference_count() << '\n';
}
#include <synapse/system/hardware/cpu.hpp>
#include <iostream>

int main()
{
    int a[10];
    std::string b;
    synapse::system::cpu::cache_prefetch(b);
}
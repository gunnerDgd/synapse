#include <synapse/lockfree/stack/fixed_stack.hpp>
#include <synapse/debug_tool/runtime_timer.hpp>

#include <iostream>
#include <thread>
#include <vector>

int main()
{
    synapse::lockfree::fixed_stack<uint64_t, 1000 * 1000> test_stack;
    for(uint64_t i = 1 ; i <= 1000 * 1000 ; i++)
        test_stack.push(i);

    uint64_t sum = 0;

    auto test_function = [&]()
    {
        uint64_t res = 0;
        for(uint64_t i = 0 ; i < 250 * 1000 ; i++)
            res += test_stack.pop();

        sum += res;
    };

    std::vector<std::thread> th_vec;
    for(int i = 0 ; i < 4 ; i++)
        th_vec.push_back(std::thread(test_function));

    for(auto& th_it : th_vec)
        th_it.join();

    std::cout << sum << std::endl;
}
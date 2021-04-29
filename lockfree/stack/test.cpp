#include <iostream>
#include <thread>

#include <synapse/lockfree/stack/stack.hpp>

int main()
{
    synapse::lockfree::stack<int> stk;
    auto tst_f = [&](int start, int end)
    {
        while(start <= end)
            stk.push(start++);
    };

    std::thread th1(tst_f, 1, 100),
    th2(tst_f, 101, 200),
    th3(tst_f, 201, 300),
    th4(tst_f, 301, 400);

    th1.join();
    th2.join();
    th3.join();
    th4.join();
}
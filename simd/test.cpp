#include <iostream>
#include <thread>
#include <atomic>

#include <unistd.h>
#include <xmmintrin.h>

int main()
{
    std::atomic<int> flag(1);
    size_t total;

    auto add_func = [&]()
    {
        while(flag.load() == 1)
            usleep(0);
    };
    
    std::thread th1(add_func), th2(add_func);
    th1.join();
    th2.join();
}
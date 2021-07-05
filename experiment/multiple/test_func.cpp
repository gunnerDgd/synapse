#include "test.hpp"
#include <iostream>
#include <thread>

int main()
{
    auto th_func = [&]() { std::cout << &a << std::endl; };
    std::thread th1(th_func), th2(th_func);

    th1.join();
    th2.join();
}
#include <iostream>
#include <string>
#include <synapse/branch/coroutine/coroutine.hpp>

void test_1(int a);

int main()
{
    std::cout << synapse::branch::coroutine::next<std::string>(test_1, 0) << std::endl;
    std::cout << synapse::branch::coroutine::next<std::string>(test_1)    << std::endl;
}

void test_1(int a)
{
    synapse::branch::coroutine::yield(std::string("Hello World #1"));
    synapse::branch::coroutine::yield(std::string("Hello World #2"));
}
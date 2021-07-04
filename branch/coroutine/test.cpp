#include <iostream>
#include <synapse/branch/coroutine/coroutine.hpp>

synapse::branch::coroutine co_test;
void test_1(int a);

int main()
{
    std::cout << co_test.execute<std::string>(test_1, 0) << std::endl;
    std::cout << co_test.advance<std::string>(test_1)    << std::endl;
}

void test_1(int a)
{
    co_test.yield(std::string("Hello World #1"));
    std::cout << "Yield\n";
    co_test.yield(std::string("Hello World #2"));
}
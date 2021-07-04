#include <iostream>
#include <synapse/branch/coroutine/coroutine.hpp>

synapse::branch::coroutine co_test;

void test_1(int a);
void test_2(int a);

int main()
{
    int test_1_yield = co_test.execute<int>(test_1, 3);
    std::cout << "Main Yield : " << test_1_yield << std::endl;
}

void test_1(int a)
{
    std::cout << "Yield : " << co_test.execute<int>(test_2, 5) << std::endl;
    co_test.exit(test_1, 10);    
}
void test_2(int a)
{
    co_test.yield(3, test_1);
}
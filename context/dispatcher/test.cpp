#include <synapse/context/dispatcher/dispatcher.hpp>
#include <iostream>

void test_1();
void test_2();

int main()
{
    context_dispatcher.switch_context(test_1, 1024);
}

void test_1()
{
    std::cout << "Hello Test 1 #1\n";
    context_dispatcher.switch_context(test_2, 1024);

    std::cout << "Hello Test 1 #2\n";
}

void test_2()
{
    std::cout << "Hello Test 2 #1\n";
    context_dispatcher.switch_context(test_1);
    std::cout << "Hello Test 2 #2\n";
}
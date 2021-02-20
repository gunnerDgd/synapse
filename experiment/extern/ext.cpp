#include <iostream>

void test_1();
int main()
{
    int a = 3;
    auto cap_call = [&]() { test_1(); };
    test_1();
    return 0;
}

void test_1()
{
    std::cout << a << std::endl;
}
#include <iostream>

enum class test_enum : int
{
    a = 1,
    b = (1 << 1)
};

int operator| (const test_enum& rhs, const test_enum& lhs) { return (int)rhs | (int)lhs; }

void print_enum(int a) { std::cout << a << std::endl; }

int main()
{
    print_enum(test_enum::a | test_enum::b);
}
#include <iostream>
#include <tuple>
#include <utility>

void test_1(int a, float b)
{
    std::cout << a << " " << b << std::endl;
}

template <typename... T>
void test(T... args)
{
    auto tst = std::make_tuple(std::move(args)...);
    std::apply(test_1, tst);
}

int main()
{
    test(1, 3.0);
}
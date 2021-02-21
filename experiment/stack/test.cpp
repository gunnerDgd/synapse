#include <iostream>

int main()
{
    uint64_t rsp = 15;
    char a = 0;
    asm volatile ("movq %%rsp, %0" : "=g"(rsp));

    std::cout << rsp % 16 << std::endl;
    std::cout << rsp << std::endl;
}
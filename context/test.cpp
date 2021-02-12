#include <synapse/context/context.hpp>
#include <iostream>

static frame::stack_frame heap_stack(64 * 1024);

void test(heap_stack)
{

}

int main()
{

    nx_stk.restore_stack();

    int a;
    std::cout << &a << std::endl;

    pr_stk.restore_stack();
    //int b;
    //std::cout << &b << std::endl;
}


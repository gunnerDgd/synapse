#include <synapse/context/context.hpp>
#include <iostream>

static frame::stack_frame  heap_stack(64 * 1024), main_stack;

void test()
{
    int a;
    std::cout << &a << std::endl;
}

int main()
{
    main_stack.save_stack   ();
    heap_stack.restore_stack(); test();

    main_stack.restore_stack();
    int b;
    std::cout << &b << std::endl;
}


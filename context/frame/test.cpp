#include <synapse/context/frame/frame.hpp>

int main()
{
    int a = 3;
    frame::stack stk;

    std::cout << a << std::endl;
    stk.stack_top = 0;
    stk.set_base_pointer();

    std::cout << a << std::endl;
}
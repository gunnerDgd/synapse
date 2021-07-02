#include <iostream>

namespace context
{
struct context
{
    uint64_t test_reg[17];
    //uint64_t test_rip    ;
};
}


extern "C" 
{
    extern void save_context(context::context*);
}

int main()
{
    context::context a;
    asm volatile ( "movq $0x10, %rbx\n\t" );
    save_context(&a);

    std::cout << a.test_reg[16] << std::endl;
    std::cout << (uint64_t)main  << std::endl;
}
#include <iostream>
#include <synapse/context/switch.hpp>

namespace stack
{
    template <typename T>
    IN_FUNC_VOID push_stack(T*&&, T*&);

    template <typename T>
    IN_FUNC_VOID pop_stack (T&);
}

template <typename T>
IN_FUNC_VOID stack::push_stack(T*&& st_ctx, T*& st_ptr)
{
    asm volatile
    (
        "push        %1\n\t"
        "movq %%rsp, %0"
    :   "=g"(*st_ptr)
    :   "g" (st_ctx)
    );
}

template <typename T>
IN_FUNC_VOID stack::pop_stack (T& st_ctx) { asm volatile("popq %0" : "=g"(st_ctx)); }
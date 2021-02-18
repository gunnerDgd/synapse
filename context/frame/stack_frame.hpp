#pragma once
#include <iostream>

#define IN_FUNC __attribute__((always_inline))

namespace frame
{
    class stack
    {
    public:
        inline IN_FUNC stack();
        inline IN_FUNC stack(size_t stack_size);

    public:
        void inline IN_FUNC set_base_pointer ();
        void inline IN_FUNC set_stack_pointer();

        void inline IN_FUNC get_base_pointer ();
        void inline IN_FUNC get_stack_pointer();

    public:
        void*    stack_entry  = nullptr;
        uint64_t stack_bottom = 0,
                 stack_top    = 0;
    };
}

inline IN_FUNC frame::stack::stack()
{
    asm volatile
    (
        "movq %%rsp, %0\n\t"
        "movq %%rbp, %1"
    :   "=g"(stack_bottom), "=g"(stack_top)
    );
}

inline IN_FUNC frame::stack::stack(size_t stack_size)
{
    stack_entry  = new uint64_t[stack_size + 8];
    stack_top    = (uint64_t)stack_entry + stack_size;
    stack_bottom = stack_top;
}

void inline IN_FUNC frame::stack::set_stack_pointer() { asm volatile("movq %0, %%rsp" :: "g"(stack_bottom)); }
void inline IN_FUNC frame::stack::set_base_pointer () { asm volatile("movq %0, %%rbp" :: "g"(stack_top))   ; }

void inline IN_FUNC frame::stack::get_stack_pointer() { asm volatile("movq %%rsp, %0" : "=g"(stack_bottom)); }
void inline IN_FUNC frame::stack::get_base_pointer () { asm volatile("movq %%rbp, %0" : "=g"(stack_top))   ; }
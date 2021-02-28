#pragma once
#include <iostream>

#define IN_FUNC __attribute__((always_inline))

/*
+-------------------+
|                   |
| Parameter Section |
|                   |
+-------------------+
+-------------------+
|                   |
|  Private Section  |
|                   |
+-------------------+
+-------------------+
|                   |
| Pre-Stack Section |
|                   |
+-------------------+ <-- %%rbp
+-------------------+
|                   |
|  Function Stack   |
|                   |
+-------------------+ <-- %%rsp

Stack Pointer (%%rsp) must be aligned as 16 Bytes.
*/

namespace frame
{
    class stack
    {
    public:
        inline IN_FUNC stack ();
        inline IN_FUNC stack (size_t stack_size);
        inline IN_FUNC ~stack();

    public:
        void inline IN_FUNC set_base_pointer ();
        void inline IN_FUNC set_stack_pointer();

        void inline IN_FUNC get_base_pointer ();
        void inline IN_FUNC get_stack_pointer();

    public:
        void            *stack_entry  = nullptr; // Always Fixed.
                                                  // Must be nullptr when the stack isn't user defined.

        uint64_t         stack_bottom = 0,
                         stack_top    = 0;
    };
}

inline IN_FUNC frame::stack::stack()
{
    get_stack_pointer();
    get_base_pointer ();
}

inline IN_FUNC frame::stack::stack(size_t stack_size)
{
    if(stack_size % 16 != 0)
        stack_size += (16 - (stack_size % 16));
    
    stack_entry  = new uint64_t[stack_size];
    
    stack_top    = (uint64_t)stack_entry + stack_size; // RBP
    stack_bottom = stack_top;                          // RSP
}

inline IN_FUNC frame::stack::~stack()
{
    if(stack_entry == nullptr) return;
    else                       delete[] (uint8_t*)stack_entry;
}

void inline IN_FUNC frame::stack::set_stack_pointer() { asm volatile("movq %0, %%rsp" :: "g"(stack_bottom)); }
void inline IN_FUNC frame::stack::set_base_pointer () { asm volatile("movq %0, %%rbp" :: "g"(stack_top))   ; }

void inline IN_FUNC frame::stack::get_stack_pointer() { asm volatile("movq %%rsp, %0" : "=g"(stack_bottom)); }
void inline IN_FUNC frame::stack::get_base_pointer () { asm volatile("movq %%rbp, %0" : "=g"(stack_top))   ; }
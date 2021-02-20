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


*/

namespace frame
{
    class stack
    {
    public:
        struct private_section  { void    * private_start, 
                                          * private_end;             };
        struct previous_section { uint64_t* previous_instruction_pointer, 
                                          * previous_base_pointer;   };
        struct function_section { void    * function_base_pointer,
                                          * function_stack_pointer;  };

    public:
        inline IN_FUNC stack();
        inline IN_FUNC stack(size_t stack_size);

    public:
        void inline IN_FUNC set_base_pointer ();
        void inline IN_FUNC set_stack_pointer();

        void inline IN_FUNC get_base_pointer ();
        void inline IN_FUNC get_stack_pointer();

    public:
        void            *stack_entry  = nullptr; // Always Fixed.
                                                  // Must be nullptr when the stack isn't user defined.

        private_section  sec_priv     = nullptr; // Must be nullptr when the stack isn't user defined.
        previous_section sec_prev     = nullptr;
        function_section sec_func     = nullptr;
    };
}

inline IN_FUNC frame::stack::stack()
{
    asm volatile
    (
        "leaq 0x00(%%rbp), %0\n\t"
        "leaq 0x08(%%rbp), %1\n\t"

        "movq %%rbp, "

    :   "=g"()
    )
}

inline IN_FUNC frame::stack::stack(size_t stack_size)
{
    stack_entry  = new uint64_t[stack_size];
    
    stack_bottom = stack_entry;                      // RSP
    stack_top    = (uint8_t*)stack_top + stack_size; // RBP
}

void inline IN_FUNC frame::stack::set_stack_pointer() { asm volatile("movq %0, %%rsp" :: "g"(stack_bottom)); }
void inline IN_FUNC frame::stack::set_base_pointer () { asm volatile("movq %0, %%rbp" :: "g"(stack_top))   ; }

void inline IN_FUNC frame::stack::get_stack_pointer() { asm volatile("movq %%rsp, %0" : "=g"(stack_bottom)); }
void inline IN_FUNC frame::stack::get_base_pointer () { asm volatile("movq %%rbp, %0" : "=g"(stack_top))   ; }
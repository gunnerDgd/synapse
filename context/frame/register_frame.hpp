#pragma once
#include <synapse/synapse.hpp>
#include <iostream>
#define IN_FUNC __attribute__((always_inline))

namespace frame
{
    class register_frame
    {
    public:
        enum index : uint16_t
        {
            rax, rbx, rcx, rdx,
            rsi, rdi,
            r8 , r9 , r10, r11, r12, r13, r14, r15,
            rip
        };

    public:
        inline           IN_FUNC register_frame  ()         { save_register(); }
      
        inline void      IN_FUNC save_register   ();
        inline void      IN_FUNC restore_register();
        
        uint64_t                 register_set[15];
    };
}

inline void IN_FUNC frame::register_frame::save_register()
{
    asm volatile
    (
        "movq %%rax, %0\n\t"
        "movq %%rbx, %1\n\t"
        "movq %%rcx, %2\n\t"
        "movq %%rdx, %3\n\t"

        "movq %%rsi, %4\n\t"
        "movq %%rdi, %5\n\t"

        "movq %%r8 , %6\n\t"
        "movq %%r9 , %7\n\t"
        "movq %%r10, %8\n\t"
        "movq %%r11, %9\n\t"
        "movq %%r12, %10\n\t"
        "movq %%r13, %11\n\t"
        "movq %%r14, %12\n\t"
        "movq %%r15, %13"
    :   "=g"(register_set[0]), "=g"(register_set[1]), "=g"(register_set[2]), "=g"(register_set[3]),
        "=g"(register_set[4]), "=g"(register_set[5]),

        "=g"(register_set[6]) , "=g"(register_set[7]) , "=g"(register_set[8]) , "=g"(register_set[9]),
        "=g"(register_set[10]), "=g"(register_set[11]), "=g"(register_set[12]), "=g"(register_set[13])
    );
}

inline void IN_FUNC frame::register_frame::restore_register()
{
    asm volatile
    (
        "movq %0 , %%rax\n\t"
        "movq %1 , %%rbx\n\t"
        "movq %2 , %%rcx\n\t"
        "movq %3 , %%rdx\n\t"

        "movq %4 , %%rsi\n\t"
        "movq %5 , %%rdi\n\t"

        "movq %6 , %%r8\n\t"
        "movq %7 , %%r9\n\t"
        "movq %8 , %%r10\n\t"
        "movq %9 , %%r11\n\t"
        "movq %10, %%r12\n\t"
        "movq %11, %%r13\n\t"
        "movq %12, %%r14\n\t"
        "movq %13, %%r15"
    ::  "g"(register_set[0]), "g"(register_set[1]), "g"(register_set[2]), "g"(register_set[3]),
        "g"(register_set[4]), "g"(register_set[5]),

        "g"(register_set[6]) , "g"(register_set[7]) , "g"(register_set[8]) , "g"(register_set[9]),
        "g"(register_set[10]), "g"(register_set[11]), "g"(register_set[12]), "g"(register_set[13])
    );
}
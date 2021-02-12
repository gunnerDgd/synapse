#pragma once
#include <iostream>

#define IN_FUNC __attribute__((always_inline))
#define this_stack static

namespace frame
{
    class stack_frame
    {
    public:
        enum index : uint16_t
        {
            current_base_pointer ,
            current_stack_pointer
        };

        inline      IN_FUNC stack_frame()                    { save_stack(); }
        inline      IN_FUNC stack_frame(uint64_t frame_size)
        {
            current_stack[0] = reinterpret_cast<uint64_t>(new uint8_t[frame_size]);
            current_stack[1] = current_stack[0] + frame_size;
        }

        inline void        IN_FUNC save_stack   ();
        inline void        IN_FUNC restore_stack();

        uint64_t                 current_stack[2];
    };
}


inline void IN_FUNC frame::stack_frame::save_stack()
{
    asm volatile
    (
        "movq %%rbp, %0\n\t"
        "movq %%rsp, %1"

    :   "=g"(current_stack[0]),
        "=g"(current_stack[1])
    );
}

inline void IN_FUNC frame::stack_frame::restore_stack()
{
    asm volatile
    (
        "leaq 0x00(%0), %%rbp\n\t"
        "movq      %1 , %%rsp"
    
    ::  "g"(&current_stack[0]),
        "g"( current_stack[1])
    );
}
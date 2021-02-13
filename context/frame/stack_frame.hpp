#pragma once
#include <iostream>

#define IN_FUNC __attribute__((always_inline))

namespace frame
{
/*
class "stack_frame"

1) This class saves current rsp, rbp.
2) Also this class can save 

*/
    class stack_frame
    {
    public:
        enum index : uint16_t
        {
            current_base_pointer ,
            current_stack_pointer,
            current_stack_size
        };

        inline      IN_FUNC stack_frame  () 
        { 
            save_stack_pointer();
            save_base_pointer ();

            current_stack[2] = current_stack[0] - current_stack[1];
        }
        inline      IN_FUNC stack_frame  (uint64_t stack_size)
        {
            current_stack[0]  = reinterpret_cast<uint64_t>(new uint8_t[stack_size]);
            current_stack[0] += stack_size;

            current_stack[1] = current_stack[0];
            current_stack[2] = stack_size;
        }

        inline void IN_FUNC save_stack_pointer()    { asm volatile ("movq %%rsp, %0" :  "=g"(current_stack[1])); }
        inline void IN_FUNC save_base_pointer ()    { asm volatile ("movq %%rbp, %0" :  "=g"(current_stack[0])); }
        
        inline void IN_FUNC restore_stack_pointer() { asm volatile ("movq %0, %%rsp" :: "g" (current_stack[1])); }
        inline void IN_FUNC restore_base_pointer () { asm volatile ("movq %0, %%rbp" :: "g" (current_stack[0])); }

        uint64_t            current_stack[3];
    };
}
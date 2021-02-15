#pragma once
#include <synapse/context/frame/previous.hpp>
#include <synapse/context/frame/register_frame.hpp>
#include <synapse/context/frame/stack_frame.hpp>

namespace frame
{
    extern frame::stack_frame   * current_stack_frame   ;
    extern frame::register_frame* current_register_frame;

    class  frame : public stack_frame, 
                   public register_frame
    {
    public:
        frame() {}
        frame(uint64_t stack_size)
            : stack_frame   (stack_size),
              register_frame() {}
    };
}
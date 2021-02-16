#pragma once
#include <synapse/context/frame/previous.hpp>
#include <synapse/context/frame/register_frame.hpp>
#include <synapse/context/frame/stack_frame.hpp>

namespace frame
{
    class frame : public stack_frame, 
                  public register_frame
    {
    public:
        frame()
            : stack_frame   (),
              register_frame() {}
              
        frame(uint64_t stack_size)
            : stack_frame   (stack_size),
              register_frame() {}
    };
}
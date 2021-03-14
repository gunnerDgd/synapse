#pragma once
#include <synapse/context/frame/cpu.hpp>
#include <synapse/context/frame/stack.hpp>

namespace synapse
{
namespace frame
{
    class frame : public stack, public cpu
    {
    public:
        frame()
            : stack(),
              cpu  () {}
              
        frame(uint64_t stack_size)
            : stack(stack_size),
              cpu  () {}
    };
}
}
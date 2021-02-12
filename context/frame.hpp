#include <synapse/context/previous.hpp>
#include <synapse/context/stack_frame.hpp>
#include <synapse/context/register_frame.hpp>

namespace frame
{
    void switch_frame(register_frame* pr_reg,
                      register_frame* nx_reg,
                      stack_frame   * nx_stk)
    {
        frame::previous prev_entity;
        pr_reg->save   ();

        static uint64_t prev_rip = *prev_entity[frame::previous::index::previous_instruction];
        nx_reg->restore();
        nx_stk->restore();

        asm volatile   ("jmpq %0" : "g"(prev_rip));
    }
}
#include <synapse/context/register_frame.hpp>
#include <synapse/context/stack_frame.hpp>
#include <synapse/context/previous.hpp>

namespace frame
{
    class context : public stack_frame,
                    public register_frame
    {
    public:
        context(uint64_t fr_stk_size)
        : frame::stack_frame   (fr_stk_size),
          frame::register_frame() {}

        context()                 {}
    };
}
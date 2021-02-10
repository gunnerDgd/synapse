#include <synapse/context/register_frame.hpp>
#include <synapse/context/stack_frame.hpp>
#include <synapse/context/previous.hpp>

namespace context
{
    struct context
    {
        context() {}

        frame::register_frame ctx_register;
        frame::stack_frame    ctx_stack   ;
        frame::previous       ctx_previous;
    };

   /* class branch
    {
    public:
        branch(branch&  br_prev    ,
               uint64_t br_stk_size = 64 * 1024)
            :  br_reg_frame(new frame::register_frame),
               br_stk_frame(new frame::stack_frame(br_stk_size))),
               br_caller   (br_prev) {}

        // Remember. Start is another type of "Switching".

        template <typename _out, typename... _in>
        void start(_out(*br_fp)(_in...), _in... br_arg);

        template <typename _out>
        void start(_out(*br_fp)(void));

    private: // Branch Information
        frame::register_frame* br_reg_frame = nullptr;
        frame::stack_frame   * br_stk_frame = nullptr;

    private: // Branch Previous Information
        branch&                br_caller;
    };*/
}
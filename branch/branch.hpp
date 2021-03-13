#include <iostream>
#include <synapse/context/switch.hpp>

namespace synapse
{
namespace branch
{
    class branch_base : public frame::frame
    {
    public:
        virtual void execute(frame::frame& prev) = 0;
    };

    template <typename _out>
    class branch<_out(void)> : public branch_base
    {
    public:
        branch(_out(&&fp)(void))
             : br_fp(fp) {}

        void execute(frame::frame& prev) override;
    
    private:
        _out(*br_fp)(void);
        bool  br_running = false;
    };

    template <typename _out, typename... _in>
    class branch<_out(_in...)> : public branch_base
    {
    public:
        branch(_out(&&fp)(_in...), _in&&... farg)
             : br_arg(std::make_tuple(std::move(farg...))),
               br_fp (fp) { }

        void execute(frame::frame& prev) override;

    private:
        std::tuple<_in...> br_arg;
        _out             (*br_fp)(_in...);
};
}
}

template <typename _out>
void synapse::branch::branch<_out(void)>::execute(frame::frame& prev)
{
    if(!br_running)
        frame::switch_frame(prev, *this, br_fp);
    else
        frame::switch_frame(prev, *this);
}

template <typename _out, typename... _in>
void synapse::branch::branch<_out(_in...)>::execute(frame::frame& prev)
{
    if(!br_running)
        frame::switch_frame(prev, *this, br_fp, br_arg);
    else
        frame::switch_frame(prev, *this);
}
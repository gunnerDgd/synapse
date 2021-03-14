#pragma once
#include <iostream>
#include <synapse/context/branch/dispatch.hpp>

namespace synapse
{
namespace branch
{
    template <typename T>
    class branch {};

    template <typename _out>
    class branch<_out(void)> : public branch_base
    {
    public:
        branch(dispatch& d, _out(&&fp)(dispatch&))
             : br_fp  (fp),
               br_disp(d) { d.start_branch(*this); }

        void execute(frame& prev) override
        {
            if(!br_running) {
                switch_frame(prev, *this, br_fp, br_disp);
                br_running = true;
            }
            else
                switch_frame(prev, *this);
        }
    
    private:
        _out    (*br_fp)(dispatch&);
        dispatch& br_disp;
        bool      br_running = false;
        
    };

    template <typename _out, typename... _in>
    class branch<_out(_in...)> : public branch_base
    {
    public:
        branch(dispatch& d, _out(&&fp)(dispatch&, _in...), _in&&... farg)
             : br_arg (std::make_tuple(std::move(farg...))),
               br_fp  (fp),
               br_disp(d) { d.start_branch(*this); }

        void execute(frame& prev) override
        {
            if(!br_running) {
                switch_frame(prev, *this, br_fp, std::tuple_cat(std::make_tuple(br_disp), br_arg));
                br_running = true;
            }
            else
                switch_frame(prev, *this);
        }

    private:
        dispatch&          br_disp;
        bool               br_running = false;

        std::tuple<_in...> br_arg ;
        _out             (*br_fp)(_in...);
};
}
}
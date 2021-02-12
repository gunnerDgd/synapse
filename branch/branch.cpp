#include "branch_dispatcher.hpp"

branch::branch_handle* branch::dispatcher::branch_current;

template <typename _out, typename... _in>
void branch::dispatcher::add_branch   (_out(*bf)(_in...), context::context* bh)
{
    branch_queue.insert(std::make_pair((uint64_t)bf, bh));
}

template <typename _out>
void branch::dispatcher::add_branch   (_out(*bf)(void)  , context::context* bh)
{
    branch_queue.insert(std::make_pair((uint64_t)bf, bh));
}

template <typename _out, typename... _in>
void branch::dispatcher::delete_branch(_out(*bf)(_in...)) { branch_queue.erase((uint64_t)bf); }

template <typename _out>
void branch::dispatcher::delete_branch(_out(*bf)(void))   { branch_queue.erase((uint64_t)bf); }

template <typename br_ca, typename br_ce> // ca : Caller, ce : Callee
void branch::dispatcher::switch_branch(br_ca _ca, br_ce _ce)
{
    frame::previous h_ca_prev;
    branch_handle*  h_ca = (*this)[_ca],
                    h_ce = (*this)[_ce];

    if  (!h_ca || !h_ce) return;
    h_ca->br_reg->save();

    (*h_ca->br_reg)[frame::register_frame::index::rip] = *h_ca_prev[frame::previous::index::previous_instruction];
    branch_current = h_ce;

}
#include <synapse/branch/coroutine/coroutine_node.hpp>
#include <functional>

namespace synapse   {
namespace branch    {
namespace coroutine {

    template <typename co_yield_t, typename R, typename... Args>
    co_yield_t next (R(*co_exec)(Args...), Args... co_args);

    template <typename co_yield_t, typename co_exec_t>
    co_yield_t next (co_exec_t co_exec);

    template <typename co_yield_t>
    void       yield(co_yield_t co_yield);

}
}
}

template <typename co_yield_t, typename co_exec_t>
co_yield_t synapse::branch::coroutine::next(co_exec_t co_exec)
{
    auto                                        co_new_it = co_current_node->co_child.find((uint64_t)co_exec);
    synapse::branch::coroutine::coroutine_node* co_new    = (*co_new_it).second;

    co_current_node              = co_new;
    synapse::branch::switch_branch(co_new->co_parent->co_branch, co_new->co_branch);

    return std::any_cast<co_yield_t>(co_yield_argument);
}

template <typename co_yield_t, typename R, typename... Args>
co_yield_t synapse::branch::coroutine::next(R(*co_exec)(Args...), Args... co_args)
{
    if(!co_thread_node.co_parent)
        co_current_node = &co_thread_node;


    auto co_new       = new synapse::branch::coroutine::coroutine_node;
    co_new->co_parent = co_current_node;
        
    co_current_node->co_child.insert(std::make_pair((uint64_t)co_exec, co_new));
    synapse::branch::store_branch   (co_current_node->co_branch);
        
    co_current_node   = co_new;
    std::apply         (co_exec, std::make_tuple(co_args...));
    
    return              std::any_cast<co_yield_t>(co_yield_argument);
}

template <typename co_yield_t>
void synapse::branch::coroutine::yield(co_yield_t co_yield)
{
    synapse::branch::coroutine::coroutine_node* co_prev = co_current_node,
                                              * co_next = co_current_node->co_parent;

    co_yield_argument = co_yield;
    co_current_node   = co_next ;

    synapse::branch::switch_branch(co_prev->co_branch, co_next->co_branch);
}
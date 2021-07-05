#include <synapse/branch/coroutine/coroutine_node.hpp>
#include <synapse/branch/context/context.hpp>
#include <functional>

namespace synapse   {
namespace branch    {
namespace coroutine {

    template <typename co_yield_t, typename R, typename... Args>
    co_yield_t next (R(*co_exec)(Args...), Args... co_args);

    template <typename co_yield_t>
    void       yield(co_yield_t co_yield);

}
}
}

template <typename co_yield_t, typename R, typename... Args>
co_yield_t synapse::branch::coroutine::next(R(*co_exec)(Args...), Args... co_args)
{
    auto   co_find  = co_current_node->co_child.find((uint64_t)co_exec);
    switch(co_find == co_current_node->co_child.end())
    {
    case true: // If the Coroutine is newly registered.
    {
        auto co_new       = new synapse::branch::coroutine::coroutine_node;
        co_new->co_parent = co_current_node;
        
        synapse::branch::store_branch(co_current_node->co_branch);
        co_current_node   = co_new;

        std::apply(co_exec, std::make_tuple(co_args...));
        break;
    }

    case false:
    {
        synapse::branch::coroutine::coroutine_node* co_prev = co_current_node;
        co_current_node                                     = *co_find;

        synapse::branch::switch_branch(co_prev->co_branch, co_current_node->co_branch);
        break;
    }
    }

    return std::any_cast<co_yield_t>(co_yield_argument);
}

template <typename co_yield_t>
void synapse::branch::coroutine::yield(co_yield_t co_yield)
{
    synapse::branch::coroutine_node* co_prev = co_current_node,
                                   * co_next = co_current_node->co_parent;

    co_yield_argument = co_yield;
    co_current_node   = co_next ;

    synapse::branch::switch_branch(*co_prev, *co_next);
}
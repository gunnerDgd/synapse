#include <synapse/branch/branch.hpp>
#include <unordered_map>
#include <any>

namespace synapse   {
namespace branch    {
    
    struct coroutine_node
    {
        coroutine_node*                               co_parent = nullptr;
        synapse::branch::branch                       co_branch          ;
        std::unordered_map<uint64_t, coroutine_node*> co_child           ;
    };

    class coroutine
    {
    public:
        coroutine()
            : coroutine_node_current(&coroutine_stack) { }

        template <typename co_yield_t, typename co_exec_t, typename... co_exec_arg_t>
        co_yield_t  execute(co_exec_t co_exec, co_exec_arg_t... co_exec_arg);

        template <typename co_yield_t, typename co_exec_t>
        co_yield_t  execute(co_exec_t co_exec);
        
        template <typename yield_t>
        void        yield  (yield_t   co_ret);

        template <typename yield_t>
        void        exit   (yield_t   co_ret);

    private:
        synapse::branch::coroutine_node  coroutine_stack       ,
                                        *coroutine_node_current;
        std::any                         coroutine_argument    ;

        template <typename co_exec_t, typename... co_exec_arg_t>
        void execute_branch(co_exec_t co_exec, co_exec_arg_t... co_exec_arg);

        template <typename co_exec_t>
        void execute_branch(co_exec_t co_exec);
    };

}
}

template <typename yield_t>
void synapse::branch::coroutine::exit(yield_t co_ret)
{
    synapse::branch::coroutine_node* co_exit = coroutine_node_current;
    coroutine_node_current                   = coroutine_node_current->co_parent;
    
    coroutine_argument                       = co_ret;
    synapse::branch::load_branch(coroutine_node_current->co_branch);
}

template <typename co_exec_t, typename... co_exec_arg_t>
void synapse::branch::coroutine::execute_branch(co_exec_t co_exec, co_exec_arg_t... co_exec_arg)
{
    synapse::branch::coroutine_node* co_new = new synapse::branch::coroutine_node;
    co_new->co_parent                       = coroutine_node_current ;
    
    coroutine_node_current->co_child.insert(std::make_pair((uint64_t)co_exec, co_new));
    coroutine_node_current = co_new;

    synapse::branch::switch_branch(co_new->co_parent->co_branch, co_exec, co_exec_arg);
}

template <typename co_exec_t>
void synapse::branch::coroutine::execute_branch(co_exec_t co_exec)
{
    synapse::branch::coroutine_node* co_run = (*coroutine_node_current->co_child.find((uint64_t)co_exec)).second;
    coroutine_node_current                  = co_run;
    
    synapse::branch::switch_branch(co_run->co_parent->co_branch, co_run->co_branch);
}

template <typename co_yield_t, typename co_exec_t, typename... co_exec_arg_t>
co_yield_t synapse::branch::coroutine::execute(co_exec_t co_exec, co_exec_arg_t... co_exec_arg)
{
    execute_branch(co_exec, co_exec_arg...);
    return         std::any_cast<co_yield_t>(coroutine_argument);
}

template <typename co_yield_t, typename co_exec_t>
co_yield_t synapse::branch::coroutine::execute(co_exec_t co_exec)
{
    execute_branch(co_exec);
    return         std::any_cast<co_yield_t>(coroutine_argument);
}

template <typename yield_t>
void synapse::branch::coroutine::yield(yield_t co_ret)
{
    synapse::branch::coroutine_node* co_prev = coroutine_node_current;
    coroutine_node_current                   = coroutine_node_current->co_parent ;
    
    coroutine_argument                       = co_ret;
    synapse::branch::switch_branch(co_prev->co_branch, coroutine_node_current->co_branch);
}
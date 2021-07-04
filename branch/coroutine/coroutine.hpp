#include <synapse/branch/branch.hpp>
#include <unordered_map>

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
        void*                            coroutine_argument    = nullptr;

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

    
    *reinterpret_cast<yield_t*> (coroutine_argument) = co_ret;
    synapse::branch::load_branch(coroutine_node_current->co_branch);
}

template <typename co_exec_t, typename... co_exec_arg_t>
void synapse::branch::coroutine::execute_branch(co_exec_t co_exec, co_exec_arg_t... co_exec_arg)
{
    synapse::branch::store_branch   (coroutine_node_current->co_branch);
    synapse::branch::coroutine_node* co_new = new synapse::branch::coroutine_node;

    co_new->co_parent     = coroutine_node_current ;
    coroutine_node_current->co_child.insert(std::make_pair((uint64_t)co_exec, co_new));

    coroutine_node_current = co_new;
    std::apply              (co_exec, std::make_tuple(co_exec_arg...));
}

template <typename co_exec_t>
void synapse::branch::coroutine::execute_branch(co_exec_t co_exec)
{
    synapse::branch::store_branch              (coroutine_node_current->co_branch);
    synapse::branch::coroutine_node* co_run = (*coroutine_node_current->co_child.find((uint64_t)co_exec)).second;
    
    coroutine_node_current         = co_run;
    synapse::branch::load_branch    (co_run->co_branch);
}

template <typename co_yield_t, typename co_exec_t, typename... co_exec_arg_t>
co_yield_t synapse::branch::coroutine::execute(co_exec_t co_exec, co_exec_arg_t... co_exec_arg)
{
    execute_branch(co_exec, co_exec_arg...);
    co_yield_t     co_yield_cp = *reinterpret_cast<co_yield_t*>(coroutine_argument);
    delete                        reinterpret_cast<co_yield_t*>(coroutine_argument);

    return         co_yield_cp;
}

template <typename co_yield_t, typename co_exec_t>
co_yield_t synapse::branch::coroutine::execute(co_exec_t co_exec)
{
    execute_branch(co_exec);
    co_yield_t     co_yield_cp = *reinterpret_cast<co_yield_t*>(coroutine_argument);
    delete                        reinterpret_cast<co_yield_t*>(coroutine_argument);

    return         co_yield_cp;
}

template <typename yield_t>
void synapse::branch::coroutine::yield(yield_t co_ret)
{
    synapse::branch::store_branch(coroutine_node_current->co_branch);

    coroutine_node_current      = coroutine_node_current->co_parent ;
    coroutine_argument          = new yield_t;

    *reinterpret_cast<yield_t*>  (coroutine_argument)       = co_ret;
    synapse::branch::load_branch (coroutine_node_current->co_branch); 
}
#include <synapse/branch/branch.hpp>
#include <unordered_map>

#define BRANCH_MAIN_COROUTINE 0x00

namespace synapse   {
namespace branch    {
    
    class coroutine
    {
    public:
        coroutine()
        {
            synapse::branch::branch* co_main_routine = new synapse::branch::branch;
            coroutine_current                        = co_main_routine;
            coroutine_stack.insert(std::make_pair(BRANCH_MAIN_COROUTINE, coroutine_current));
        }

        template <typename co_yield_t, typename co_exec_t, typename... co_exec_arg_t>
        co_yield_t  execute(co_exec_t co_exec, co_exec_arg_t... co_exec_arg);

        template <typename co_yield_t, typename co_exec_t>
        co_yield_t  execute(co_exec_t co_exec);
        
        template <typename yield_t, typename co_exec_t>
        void        yield  (yield_t   co_ret, co_exec_t co_exec);

        template <typename co_exec_t, typename co_yield_t>
        void        exit   (co_exec_t co_exec, co_yield_t co_ret);

    private:
        std::unordered_map<uint64_t, synapse::branch::branch*> coroutine_stack   ;
        template <typename co_exec_t, typename... co_exec_arg_t>
        void execute_branch(co_exec_t co_exec, co_exec_arg_t... co_exec_arg);

        template <typename co_exec_t>
        void execute_branch(co_exec_t co_exec);

    private:
        synapse::branch::branch*                               coroutine_current ;
        void*                                                  coroutine_argument;
    };

}
}

template <typename co_exec_t, typename co_yield_t>
void synapse::branch::coroutine::exit(co_exec_t co_exec, co_yield_t co_ret)
{
    synapse::branch::branch* co_main = (*coroutine_stack.find(0)).second,
                           * co_exit = (*coroutine_stack.find((uint64_t)co_exec)).second;
    
    delete                             co_exit;
    coroutine_stack   .erase((uint64_t)co_exec);
    
    delete                         coroutine_argument;
    *reinterpret_cast<co_yield_t*>(coroutine_argument) = co_ret;
    
    coroutine_current           = co_main ;
    synapse::branch::load_branch(*co_main);
}

template <typename co_exec_t, typename... co_exec_arg_t>
void synapse::branch::coroutine::execute_branch(co_exec_t co_exec, co_exec_arg_t... co_exec_arg)
{
    synapse::branch::store_branch(*coroutine_current);
    synapse::branch::branch*       co_new = new synapse::branch::branch;
    coroutine_current                     = co_new;

    coroutine_stack.insert(std::make_pair((uint64_t)co_exec, co_new));
    std::apply   (co_exec, std::make_tuple(co_exec_arg...));
}

template <typename co_exec_t>
void synapse::branch::coroutine::execute_branch(co_exec_t co_exec)
{
    synapse::branch::store_branch(*coroutine_current);
    synapse::branch::branch       *co_next = (*coroutine_stack.find((uint64_t)co_exec)).second;
    
    coroutine_current            = co_next ;
    synapse::branch::load_branch (*co_next);
}

template <typename co_yield_t, typename co_exec_t, typename... co_exec_arg_t>
co_yield_t synapse::branch::coroutine::execute(co_exec_t co_exec, co_exec_arg_t... co_exec_arg)
{
    execute_branch(co_exec, co_exec_arg...);
    return        *reinterpret_cast<co_yield_t*>(coroutine_argument);
}

template <typename co_yield_t, typename co_exec_t>
co_yield_t synapse::branch::coroutine::execute(co_exec_t co_exec)
{
    execute_branch(co_exec);
    return        *reinterpret_cast<co_yield_t*>(coroutine_argument);
}

template <typename yield_t, typename co_exec_t>
void synapse::branch::coroutine::yield(yield_t co_ret, co_exec_t co_exec)
{
    delete coroutine_argument;
    
    coroutine_argument            = new yield_t;
    *(yield_t*)coroutine_argument = co_ret     ;

    synapse::branch::branch       *co_next = (*coroutine_stack.find((uint64_t)co_exec)).second;
    synapse::branch::store_branch(*coroutine_current);

    coroutine_current            = co_next ;
    synapse::branch::load_branch (*co_next);
}
#include <synapse/branch/context/context_block.hpp>
#include <tuple>

extern "C"
{
    extern void load_context           (synapse::branch::context& context);
    extern void store_context          (synapse::branch::context& context);

    extern void store_execution_context(synapse::branch::context& context);
    extern void load_execution_context (synapse::branch::context& context);
}

namespace synapse {
namespace branch  {

    void store_branch  (synapse::branch::context& context) 
    {
        store_context          (context);
        store_execution_context(context);
    }

    void load_branch   (synapse::branch::context& context)
    {
        load_context          (context);
        load_execution_context(context);
    }

    // Context Switch from prev to next.
    void switch_branch (synapse::branch::context& prev, synapse::branch::context& next)
    {
        store_context          (prev);
        store_execution_context(prev);

        load_context           (next);
        load_execution_context (next);
    }

    template <typename exec_t, typename... exec_args_t>
    void switch_branch(synapse::branch::context& prev, exec_t next, exec_args_t... next_args)
    {
        store_context          (prev);
        store_execution_context(prev);

        std::apply             (next, std::make_tuple(next_args...));
    }

}
}
#include <synapse/branch/context/context_block.hpp>
#define  BRANCH_PRESERVE_STACK __attribute__((always_inline))

extern "C"
{
    extern void load_context           (synapse::branch::context& context);
    extern void store_context          (synapse::branch::context& context);

    extern void store_execution_context(synapse::branch::context& context);
    extern void load_execution_context (synapse::branch::context& context);
}

namespace synapse {
namespace branch  {

    void BRANCH_PRESERVE_STACK store_branch(synapse::branch::context& context) 
    {
        store_context          (context);
        store_execution_context(context);
    }

    void BRANCH_PRESERVE_STACK load_branch (synapse::branch::context& context) 
    {
        load_context          (context);
        load_execution_context(context);
    }

}
}
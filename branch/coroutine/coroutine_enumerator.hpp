#include <synapse/branch/coroutine/coroutine.hpp>
#include <stack>

namespace synapse   {
namespace branch    {
namespace coroutine {

    class enumerator
    {
    public:
        template <typename co_return_t, typename... co_arg_t,  typename yield_t>
        friend yield_t execute(synapse::branch::coroutine::enumerator& co_enum,
                               co_return_t                           (*co_exec)(co_arg_t...));
        
        template <typename yield_t>
        friend void    yield  (synapse::branch::coroutine::enumerator& co_enum,
                               yield_t                                 co_ret);

    private:
        std::stack<synapse::branch::context> coroutine_stack   ;
        void*                                coroutine_argument;
    };

}
}
}

template <typename co_return_t, typename... co_arg_t, typename yield_t>
void synapse::branch::coroutine::enumerator::execute(synapse::branch::coroutine::enumerator& co_enum,
                                                     co_return_t                           (*co_exec)(co_arg_t...))
{
    auto co_top = coroutine_stack.top();

    synapse::branch::store_context          (co_top);
    synapse::branch::store_execution_context(co_top);

    
}
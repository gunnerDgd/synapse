#include <synapse/branch/context/context.hpp>

namespace synapse   {
namespace branch    {
namespace coroutine {

    class coroutine : public synapse::branch::context
    {
    public:
        coroutine()
        {
            synapse::branch::store_context          (*this);
            synapse::branch::store_execution_context(*this);
        }
    };

}
}
}
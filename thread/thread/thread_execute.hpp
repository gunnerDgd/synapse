#include <synapse/synapse.hpp>

namespace synapse {
namespace thread  {

    template <typename... Args>
    struct execute_args<void(Args...)>
    {
        std::tuple<Args...> exec_argument;
        void              (*exec_function)(Args...);
    };

    template <typename R, typename... Args>
    struct execute_args<R(Args...)>
    {
        R                   exec_return_value;
        std::tuple<Args...> exec_argument;
        R                 (*exec_function)(Args...);
    }; 
}
}
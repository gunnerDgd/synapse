#include <synapse/synapse.hpp>

#include <functional>
#include <tuple>

namespace synapse {
namespace branch  {

    template <typename R, typename... Args>
    class branch_launcher
    {
    public:
        std::tuple<Args...> launcher_argument;
        
    }

    template <typename R, typename... Args>
    class branch
    {
    public:
        branch(std::function<R(Args...)> br_exec, Args... br_args)
            : branch_executor(br_exec)                   ,
              branch_argument(std::make_tuple(br_args...)) { }

        R operator() (void) { return }


    protected:
        std::tuple<Args...>       branch_argument;
        std::function<R(Args...)> branch_executor;
    };
}
}
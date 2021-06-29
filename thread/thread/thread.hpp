#include <synapse/thread/work.hpp>

namespace synapse {
namespace thread  {

    template <typename... Args>
    class thread<void(Args...)> : public work_base
    {
         
    };

    template <typename R, typename... Arg>
    class thread<R(Arg...)> : public work_base
    {
    public:
        using exec_args = std::condition<std::is_void<R>,
                                         std::condition<std::is_void<Arg...>, executor_argument<void(Arg...)>, exec_argument<void(void)>>,
                                         std::condition<std::is_void<Arg...>, executor_argument<R(Arg...)>   , exec_argument<R(void)>>>  ;
    public:
        thread(R(*th_work)(Arg...), Arg... th_args)

    protected:
        void      thread_executor(void* th_arg);
        exec_args thread_exec_args;
    };
}
}

template <typename R, typename... Arg>
void synapse::thread::thread<R(Arg...)>::thread_executor(void* th_arg)
{
    synapse::thread::thread<R(Arg...)>::exec_args* args = (synapse::thread::thread<R(Arg...)>::exec_args*)th_arg;
    args->execute();
}
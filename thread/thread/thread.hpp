#include <synapse/thread/work.hpp>

namespace synapse {
namespace thread  {

    template <typename T>
    struct executor_argument { };

    template <>
    struct executor_argument<void(void)>
    {
        void(*exec_function)(void);
        void  execute() { exec_function(); }
    };

    template <typename R>
    struct executor_argument<R(void)>
    {
        R(*exec_function)(void);
        R  exec_result;

        void execute() { R = exec_function(); }
    };

    template <typename... Arg>
    struct executor_argument<void(Arg...)>
    {
        std::tuple<Arg...> exec_argument;
        void             (*exec_function)(Arg...);

        void execute() { std::apply(exec_function, exec_argument); }
    };


    template <typename R, typename... Arg>
    struct executor_argument<R(Arg...)>
    {
        std::tuple<Arg...> exec_argument;
        R                (*exec_function)(Arg...);
        R                  exec_result  ;

        void execute() { exec_result = std::apply(exec_function, exec_argument); }
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
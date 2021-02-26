#include <synapse/context/switch.hpp>
#include <map>

namespace frame
{
    class branch_base : public frame 
    { 
    public:
        branch_base        (uint64_t stack_size = 64 * 1024);
        virtual void launch() = 0;
    };
    
    template <typename T, typename... Args>
    class branch : public branch_base
    {
    public:
        branch     (T br_fp, Args... br_args);
        void launch();

    private:
        std::tuple<Args> launch_argument;
        T                launch_pointer ;
    };
}

frame::branch_base::branch_base(uint64_t stack_size)
    : frame(stack_size) {}

template <typename T, typename... Args>
frame::branch::branch(T br_fp, Args... br_args)
    : launch_argument(br_args...),
      launch_pointer (br_fp) {}

template <typename T, typename... Args>
void frame::branch::launch()
{
    switch_pointer(*this, );
}
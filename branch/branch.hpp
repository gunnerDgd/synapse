#include <synapse/branch/branch_dispatcher.hpp>
#include <tuple>

namespace branch
{
    template <typename _out, typename... _in>
    class branch : public context::context
    {
    public:
        branch(_out(*br_fp)(_in...), _in... br_arg);

    private:
        std::tuple<_in> br_argument;
        _out          (*br_entry)(_in...);
    };

    template <typename _out>
    class branch : public context::context
    {
    public:
        branch(_out(*br_fp)(void));

    private:
        _out          (*br_entry)(void);
    };
}

template <typename _out, typename... _in>
branch::branch::branch(_out(*br_fp)(_in...), _in... br_arg)
{

}

template <typename _out>
branch::branch::branch(_out(*br_fp)(void))
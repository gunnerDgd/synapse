#include <synapse/context/dispatcher/dispatcher.hpp>

namespace branch
{
    class branch : public frame::frame
    {
    public:
        template <typename _out, typename... _in>
        branch(_out(*br_fp)(_in...), _in... br_arg)
        {
            context_dispatcher.add_context((uint64_t)br_fp, this);
            br_fp                         (std::forward<_in>(br_arg));
        }
    };
}
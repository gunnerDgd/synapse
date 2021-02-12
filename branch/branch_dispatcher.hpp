#include <synapse/context/context.hpp>

namespace branch
{
    class dispatcher
    { 
    public:
        template <typename _out, typename... _in>
        void add_branch   (_out(*bf)(_in...), context::context* bh);

        template <typename _out>
        void add_branch   (_out(*bf)(void)  , context::context* bh);

        template <typename _out, typename... _in>
        void delete_branch(_out(*bf)(_in...));

        template <typename _out>
        void delete_branch(_out(*bf)(void));

        template <typename br_ca, typename br_ce> // ca : Caller, ce : Callee
        void switch_branch(br_ca _ca, br_ce _ce);

        template <typename _out, typename... _in>
        branch_handle* operator[](_out(*fp)(_in...))
        {
            auto    it = branch_queue.find((uint64_t)fp);
            return (it == branch_queue.end()) ? it.second
                                              : nullptr;
        }
        
        template <typename _out>
        branch_handle* operator[](_out(*fp)(void))
        {
            auto    it = branch_queue.find((uint64_t)fp);
            return (it == branch_queue.end()) ? it.second
                                              : nullptr;
        }

    private:
        using br_queue = std::map<uint64_t, context::context*>;
        br_queue                            branch_queue;
        static thread_local branch_handle*  branch_current;
    };
}
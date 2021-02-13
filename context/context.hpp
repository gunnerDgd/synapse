#include <iostream>
#include <synapse/context/frame/frame.hpp>

#include <list>
#include <map>

namespace context
{
    class context : public frame::register_frame,
                    public frame::stack_frame
    {
    public:
        context()                                              {}
        context(uint64_t stack_size) : stack_frame(stack_size) {}

    public:
        inline void IN_FUNC save_context   ();
        inline void IN_FUNC restore_context();
    };

    extern std::map<uint64_t, context::context> ctx_queue;
    class  dispatcher
    {
    public:
        template <typename _out, typename... _in>
        void add_context   (_out(*ctx_fp)(_in...), _in... ctx_arg);

        template <typename _out>
        void add_context   (_out(*ctx_fp)(void));

        template <typename _out, typename... _in>
        void delete_context(_out(*ctx_fp)(_in...));

        template <typename _out>
        void delete_context(_out(*ctx_fp)(void));

        void switch_context();

    private:

    };
}

inline void IN_FUNC context::context::save_context   ()
{
    save_register     ();
            
    save_stack_pointer();
    save_base_pointer ();
}

inline void IN_FUNC context::context::restore_context()
{
    restore_register     ();
        
    restore_stack_pointer();
    restore_base_pointer ();
}
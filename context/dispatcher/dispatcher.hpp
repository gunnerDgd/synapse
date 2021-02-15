#include <synapse/context/frame/frame.hpp>
#include <map>

namespace context
{
    class  dispatcher
    {
    private:
        template <typename _out, typename... _in>
        void add_context   (_out(*disp_fp)(_in...), frame::frame* disp_fr);

        template <typename _out>
        void add_context   (_out(*disp_fp)(void)  , frame::frame* disp_fr);
        
        template <typename _out, typename... _in>
        void delete_context(_out(*disp_fp)(_in...));

        template <typename _out>
        void delete_context(_out(*disp_fp)(void))  ;

    public: // Guess to use SFINAE...
        template <typename T>
        void switch_context(T disp_fp);

        template <typename _out, typename... _in>
        void switch_context(_out(*disp_fp)(_in...), _in... disp_arg, uint64_t stack_size);

        template <typename _out>
        void switch_context(_out(*disp_fp)(void),                    uint64_t stack_size);

    private:
        frame::frame*                     disp_curr = nullptr;
        std::map<uint64_t, frame::frame*> disp_rq;
    };
}

extern thread_local context::dispatcher context_dispatcher;
extern thread_local uint64_t            context_instruction_pointer;

template <typename _out, typename... _in>
void context::dispatcher::add_context   (_out(*disp_fp)(_in...), frame::frame* disp_fr) { disp_rq.insert(std::make_pair((uint64_t)disp_fp, disp_fr)); }

template <typename _out>
void context::dispatcher::add_context   (_out(*disp_fp)(void)  , frame::frame* disp_fr) { disp_rq.insert(std::make_pair((uint64_t)disp_fp, disp_fr)); }
        
template <typename _out, typename... _in>
void context::dispatcher::delete_context(_out(*disp_fp)(_in...)) { disp_rq.erase(disp_fp); }

template <typename _out>
void context::dispatcher::delete_context(_out(*disp_fp)(void))   { disp_rq.erase(disp_fp); }

template <typename T>
void context::dispatcher::switch_context(T disp_fp)
{
    if(!disp_curr) return;

    disp_curr->save_register();
    asm volatile            ("movq 0x08(%%rbp), %0" : "=g"(disp_curr->register_set[14])); // Save Instruction Pointer.

    auto ce_it  = disp_rq.find((uint64_t)disp_fp);
    if  (ce_it == disp_rq.end()) return;

    context_instruction_pointer = ce_it->second->register_set[14];
    disp_curr                   = ce_it->second;

    disp_curr->restore_register     ();
    disp_curr->restore_stack_pointer();
    disp_curr->restore_base_pointer ();

    asm volatile  ("jmpq *%0" :: "g"(context_instruction_pointer));
}

template <typename _out, typename... _in>
void context::dispatcher::switch_context(_out(*disp_fp)(_in...), _in... disp_arg, uint64_t stack_size)
{
    frame::frame* disp_fr = (stack_size != -1) ? new frame::frame(stack_size)
                                               : new frame::frame;
    add_context(disp_fp, disp_fr);
    disp_fr   ->restore_stack_pointer();

    if(disp_curr)
        asm volatile ("movq 0x08(%%rbp), %0" : "=g"(disp_curr->register_set[14]));

    disp_curr = disp_fr;
    disp_fp   (std::forward<_in...>(disp_arg...));
}

template <typename _out>
void context::dispatcher::switch_context(_out(*disp_fp)(void)                   , uint64_t stack_size)
{
    frame::frame* disp_fr = (stack_size != -1) ? new frame::frame(stack_size)
                                               : new frame::frame;
    add_context  (disp_fp, disp_fr);
    disp_fr   ->restore_stack_pointer();

    if(disp_curr)
        asm volatile ("movq 0x08(%%rbp), %0" : "=g"(disp_curr->register_set[14]));

    disp_curr = disp_fr;
    disp_fp    ();
}
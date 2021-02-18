#include <synapse/context/frame/frame.hpp>
#include <map>

namespace frame
{
    class dispatcher
    {
    public:
        friend class stack;
        friend class cpu  ;
        friend class frame;

    public:
        class dispatcher_entity
        {
        public:
            frame     * current_frame;
            dispatcher* current_dispatcher;
        };

    public:
        template <typename _out, typename... _in>
        inline void IN_FUNC add_frame   (_out                         (*fr_fp)(dispatcher::dispatcher_entity*, _in...),
                                         _in...                         fr_fp_arg     ,
                                         dispatcher::dispatcher_entity* fr_curr       ,
                                         uint64_t                       fr_stk_size = 64 * 1024);

       template <typename _out>
        inline void IN_FUNC add_frame   (_out                         (*fr_fp)(dispatcher::dispatcher_entity*),
                                         dispatcher::dispatcher_entity* fr_curr       ,
                                         uint64_t                       fr_stk_size = 64 * 1024);

        template <typename T>
        inline void IN_FUNC switch_frame(T ce_fp, dispatcher::dispatcher_entity* ca_fr);
        
        template <typename T>
        void delete_frame(T fr_fp) { disp_rq.erase((uint64_t)fr_fp); }

    private:
        template <typename _out, typename... _in>
        void add_frame   (uint64_t                       fr_stk_size, 
                          dispatcher::dispatcher_entity* fr_curr,
                          _out                         (*fr_fp)(dispatcher::dispatcher_entity*, _in...),
                          _in...                         fr_fp_arg);

        template <typename _out>
        void add_frame   (uint64_t                       fr_stk_size,
                          dispatcher::dispatcher_entity* fr_curr,
                          _out                         (*fr_fp)(dispatcher::dispatcher_entity*));
        
        template <typename T>
        void switch_frame(dispatcher::dispatcher_entity* ca_fr,
                          T                              ce_fp);

    private:
        std::map<uint64_t, dispatcher_entity*> disp_rq;
    };
}

// External Functions, Which is exposed to users.

template <typename _out>
inline void IN_FUNC frame::dispatcher::add_frame   (_out                         (*fr_fp)(dispatcher::dispatcher_entity*),
                                                    dispatcher::dispatcher_entity* fr_curr       ,
                                                    uint64_t                       fr_stk_size)
{
    fr_curr->current_frame->save   ();
    fr_curr->current_frame->get_stack_pointer();
    fr_curr->current_frame->get_base_pointer ();

    add_frame                      (fr_stk_size, fr_curr, fr_fp);
    fr_curr->current_frame->restore();

}

template <typename _out, typename... _in>
inline void IN_FUNC frame::dispatcher::add_frame(_out                         (*fr_fp)(dispatcher::dispatcher_entity*, _in...),
                                                 _in...                         fr_fp_arg     ,
                                                 dispatcher::dispatcher_entity* fr_curr       ,
                                                 uint64_t                       fr_stk_size)
{
    fr_curr->current_frame->save();
    fr_curr->current_frame->get_stack_pointer();
    fr_curr->current_frame->get_base_pointer ();

    add_frame                      (fr_stk_size, fr_curr, fr_fp, fr_fp_arg...);
    fr_curr->current_frame->restore();
}

// Internal Functions.

template <typename _out, typename... _in>
void frame::dispatcher::add_frame   (uint64_t                       fr_stk_size,
                                     dispatcher::dispatcher_entity* fr_curr,
                                     _out                         (*fr_fp)(dispatcher::dispatcher_entity*, _in...),
                                     _in...                         fr_fp_arg)
{
    dispatcher::dispatcher_entity* fr_ent_new = new dispatcher::dispatcher_entity;
    fr_ent_new->current_frame                 = new frame(fr_stk_size);
    fr_ent_new->current_dispatcher            = this;

    asm volatile  ("movq 0x08(%%rbp), %0" : "=g"(fr_ent_new->current_frame->register_set[14])); // Save Instruction Pointer
    disp_rq.insert(std::make_pair((uint64_t)fr_fp, fr_ent_new));

    fr_ent_new->current_frame->set_stack_pointer();
    fr_ent_new->current_frame->set_base_pointer ();

    fr_fp  (fr_ent_new, std::forward<_in>(fr_fp_arg)...);
}

template <typename _out>
void frame::dispatcher::add_frame   (uint64_t                       fr_stk_size,
                                     dispatcher::dispatcher_entity* fr_curr,
                                     _out                         (*fr_fp)(dispatcher::dispatcher_entity*))
{
    dispatcher::dispatcher_entity* fr_ent_new = new dispatcher::dispatcher_entity;
    fr_ent_new->current_frame                 = new frame(fr_stk_size);
    fr_ent_new->current_dispatcher            = this;

    asm volatile  ("movq 0x08(%%rbp), %0" : "=g"(fr_ent_new->current_frame->register_set[14])); // Save Instruction Pointer
    disp_rq.insert(std::make_pair((uint64_t)fr_fp, fr_ent_new));

    fr_ent_new->current_frame->set_stack_pointer();
    fr_fp                     (fr_ent_new);
}

template <typename T>
inline void IN_FUNC frame::dispatcher::switch_frame(T ce_fp, dispatcher::dispatcher_entity* ca_fr)
{
    ca_fr->current_frame->save  ();
    ca_fr->current_frame->get_stack_pointer();
    ca_fr->current_frame->get_base_pointer ();

    switch_frame                 (ca_fr, ce_fp);
    ca_fr->current_frame->restore();
}

template <typename T>
void frame::dispatcher::switch_frame(dispatcher::dispatcher_entity* ca_fr,
                                     T                              ce_fp)
{
    asm volatile ("movq 0x08(%%rbp), %0" : "=g"(ca_fr->current_frame->register_set[14]));
    
    auto ce_it  = disp_rq.find((uint64_t)ce_fp);
    if  (ce_it == disp_rq.end()) return;

    dispatcher::dispatcher_entity* ce_ent = ce_it->second;

    asm volatile ("movq %0, %%rbx" :: "g"(ce_ent->current_frame->register_set[14]) 
                                    : "rbx");

    ce_ent->current_frame->set_stack_pointer();
    ce_ent->current_frame->set_base_pointer ();

    asm volatile ("jmpq *%%rbx" ::: "rbx");
}
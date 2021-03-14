#include <synapse/context/frame/frame.hpp>
#include <tuple>
#include <utility>

namespace synapse
{
namespace frame
{
    void switch_frame  (frame& fr_ca, frame& fr_ce); // fr_ca : Caller's Frame, fr_ce : Callee's Frame
    void switch_frame  (frame& fr_ce);

    template <typename _out, typename... _in>
    void switch_frame  (frame& fr_ca, frame& fr_ce, _out(*fp_ce)(_in...), _in&&... fp_arg);

    template <typename _out, typename... _in>
    void switch_frame  (frame& fr_ca, frame& fr_ce, _out(*fp_ce)(_in...), std::tuple<_in...> fp_arg);
    
    template <typename _out, typename    _in>
    void switch_frame  (frame& fr_ca, frame& fr_ce, _out(*fp_ce)(_in)   , _in&&    fp_arg);
    
    template <typename _out>
    void switch_frame  (frame& fr_ca, frame& fr_ce, _out(*fp_ce)(void));

// Internal Function Series, which performs actual context switching.

    void _switch_frame (frame& fr_ca, frame& fr_ce);

    template <typename _out, typename... _in>
    void _switch_frame (frame& fr_ca, frame& fr_ce, _out(*fp_ce)(_in...), _in&&... fp_arg);

    template <typename _out, typename... _in>
    void _switch_frame (frame& fr_ca, frame& fr_ce, _out(*fp_ce)(_in...), std::tuple<_in...>& fp_arg);
    
    template <typename _out, typename    _in>
    void _switch_frame (frame& fr_ca, frame& fr_ce, _out(*fp_ce)(_in)   , _in&&    fp_arg);
    
    template <typename _out>
    void _switch_frame (frame& fr_ca, frame& fr_ce, _out(*fp_ce)(void));
}
}

using namespace synapse;
void frame::switch_frame (frame& fr_ce)
{
    asm volatile ( "movq %0, %%rbx"
                :: "g"(fr_ce.register_set[14]) : "rbx");
    
    fr_ce.set_stack_pointer();
    fr_ce.set_base_pointer ();

    asm volatile ("jmpq *%%rbx" ::: "rbx");
}

void frame::switch_frame (frame& fr_ca, frame& fr_ce)
{
    fr_ca.save();
    fr_ca.get_base_pointer ();
    fr_ca.get_stack_pointer();

    _switch_frame (fr_ca, fr_ce);
    fr_ca.restore();
}

template <typename _out, typename... _in>
void frame::switch_frame (frame& fr_ca, frame& fr_ce, _out(*fp_ce)(_in...), _in&&... fp_arg)
{
    fr_ca.save();
    fr_ca.get_base_pointer ();
    fr_ca.get_stack_pointer();

    _switch_frame (fr_ca, fr_ce, fp_ce, fp_arg...);
    fr_ca.restore();
}

template <typename _out, typename... _in>
void frame::switch_frame  (frame& fr_ca, frame& fr_ce, _out(*fp_ce)(_in...), std::tuple<_in...> fp_arg)
{
    fr_ca.save();
    fr_ca.get_base_pointer ();
    fr_ca.get_stack_pointer();

    _switch_frame(fr_ca, fr_ce, fp_ce, fp_arg);
    fr_ca.restore();    
}
    
template <typename _out, typename    _in>
void frame::switch_frame  (frame& fr_ca, frame& fr_ce, _out(*fp_ce)(_in)   , _in&&    fp_arg)
{
    fr_ca.save();
    fr_ca.get_base_pointer ();
    fr_ca.get_stack_pointer();

    _switch_frame (fr_ca, fr_ce, fp_ce, fp_arg);
    fr_ca.restore();
}
    
template <typename _out>
void frame::switch_frame  (frame& fr_ca, frame& fr_ce, _out(*fp_ce)(void))
{
    fr_ca.save();
    fr_ca.get_base_pointer ();
    fr_ca.get_stack_pointer();

    _switch_frame (fr_ca, fr_ce, fp_ce);
    fr_ca.restore();
}


/*
"_switch_frame" function series

1) This Function performs "actual" context switching,
   which saves previous instruction pointer, and switches stack frame to callee's.

2) template <... , typename... _in>
   A template'ed _switch_function provides three function model.
   - No parameter
   - Only one parameter
   - Multiple parameter

   This series calls std::forward to deliver parameter(s),
   which are saved at callee's stack.

*/

template <typename _out, typename... _in>
void frame::_switch_frame (frame& fr_ca, frame& fr_ce, _out(*fp_ce)(_in...), _in&&... fp_arg)
{
    asm volatile ( "movq 0x08(%%rbp), %0"
                 : "=g"(fr_ca.register_set[14])); // Save Caller's Instruction Pointer to frame::cpu.

    fr_ce.set_stack_pointer();
    fp_ce                  (std::forward<_in>(fp_arg)...);
}
    
template <typename _out, typename... _in>
void frame::_switch_frame (frame& fr_ca, frame& fr_ce, _out(*fp_ce)(_in...), std::tuple<_in...>& fp_arg)
{
    asm volatile ( "movq 0x08(%%rbp), %0"
                 : "=g"(fr_ca.register_set[14])); // Save Caller's Instruction Pointer to frame::cpu.

    fr_ce.set_stack_pointer();
    std::apply             (fp_ce, fp_arg);
}

template <typename _out, typename    _in>
void frame::_switch_frame (frame& fr_ca, frame& fr_ce, _out(*fp_ce)(_in)   , _in&&    fp_arg)
{
    asm volatile ( "movq 0x08(%%rbp), %0"
                 : "=g"(fr_ca.register_set[14]));

    fr_ce.set_stack_pointer();
    fp_ce                  (std::forward<_in>(fp_arg));
}
    
template <typename _out>
void frame::_switch_frame (frame& fr_ca, frame& fr_ce, _out(*fp_ce)(void))
{
    asm volatile ( "movq 0x08(%%rbp), %0"
                 : "=g"(fr_ca.register_set[14]));

    fr_ce.set_stack_pointer();
    fp_ce                  ();
}

void frame::_switch_frame (frame& fr_ca, frame& fr_ce)
{
    // 1) Save Previous (switch_frame) Context's Instruction Pointer
    // 2) Save Next Context's Instruction Pointer to RBX Register.

    asm volatile ( "movq 0x08(%%rbp), %0\n\t" 
                   "movq %1         , %%rbx "
                 : "=g"(fr_ca.register_set[14])
                 : "g" (fr_ce.register_set[14]) : "rbx");

    fr_ce.set_stack_pointer();
    fr_ce.set_base_pointer ();

    asm volatile("jmpq *%%rbx" ::: "rbx");
}
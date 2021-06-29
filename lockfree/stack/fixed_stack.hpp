#pragma once
#include <synapse/synapse.hpp>
#include <synapse/lockfree/block.hpp>

#include <xmmintrin.h>

namespace synapse  {
namespace lockfree {
	
	template <typename T, size_t N>
	class fixed_stack
	{
	public:
        fixed_stack();
		
		void   push (T&  pu_ctx);
		void   push (T&& pu_ctx);
		T&     pop  ()		    ;

	private:
        T   			fixed_stack_context[N];
		std::atomic<T*> fixed_stack_pointer   ;

	private:
		T*				fixed_stack_start = fixed_stack_context		   ,
	     *			    fixed_stack_end   = &fixed_stack_context[N];
	};
}
}

template <typename T, size_t N>
synapse::lockfree::fixed_stack<T, N>::fixed_stack()
			: fixed_stack_pointer(fixed_stack_context)  { }

template <typename T, size_t N>
void synapse::lockfree::fixed_stack<T, N>::push(T& pu_ctx)
{
    T* push_block;
	do
	{
        do
        {
		    push_block        = fixed_stack_pointer.load();
        }   while(push_block == fixed_stack_end);
        
	} while (!fixed_stack_pointer.compare_exchange_weak(push_block               ,
                                                        push_block + 1           ,
												        std::memory_order_release,
												        std::memory_order_relaxed));

    *push_block = pu_ctx;
}

template <typename T, size_t N>
void synapse::lockfree::fixed_stack<T, N>::push(T&& pu_ctx)
{
    T* push_block;
	do
	{
        do
        {
		    push_block        = fixed_stack_pointer.load();
        }   while(push_block == fixed_stack_end)   		  ;
        
	} while (!fixed_stack_pointer.compare_exchange_weak(push_block               ,
                                                        push_block + 1           ,
												        std::memory_order_release,
												        std::memory_order_relaxed));

    *push_block = pu_ctx;
}


template <typename T, size_t N>
T& synapse::lockfree::fixed_stack<T, N>::pop()
{
	T* po_res;
	do
	{
		do
		{
			po_res        = fixed_stack_pointer.load();
		}   while(po_res == fixed_stack_start);

	} while (!fixed_stack_pointer.compare_exchange_weak(po_res		             ,
												        po_res - 1               ,
												        std::memory_order_release,
												        std::memory_order_relaxed));

	return *(po_res - 1);
}
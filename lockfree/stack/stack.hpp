#pragma once
#include <synapse/synapse.hpp>
#include <synapse/lockfree/block.hpp>

#include <xmmintrin.h>

namespace synapse  {
namespace lockfree {
	
	template <typename T>
	class stack
	{
	public:
		stack()
			: stack_entry(nullptr) { }
		
		void push (T&  pu_ctx);
		void push (T&& pu_ctx);
		void push (synapse::lockfree::block<T>& pu_ctx);

		T&   pop  ();

	private:
		std::atomic<block<T>*> stack_entry;
	};
}
}

template <typename T>
void synapse::lockfree::stack<T>::push (synapse::lockfree::block<T>& pu_ctx)
{
	do
	{
		pu_ctx->block_next = stack_entry.load();
	} while (!stack_entry.compare_exchange_weak(pu_ctx->block_next,
												pu_ctx	      	  ,
												std::memory_order_release,
												std::memory_order_relaxed));
}

template <typename T>
void synapse::lockfree::stack<T>::push(T& pu_ctx)
{
	block<T>* bl_new 	  = new block<T>(pu_ctx);
	bl_new->block_context = pu_ctx;

	do
	{
		bl_new->block_next = stack_entry.load();
	} while (!stack_entry.compare_exchange_weak(bl_new->block_next,
												bl_new	      	  ,
												std::memory_order_release,
												std::memory_order_relaxed));
}

template <typename T>
void synapse::lockfree::stack<T>::push(T&& pu_ctx)
{
	block<T>* bl_new 	  = new block<T>(pu_ctx);
	bl_new->block_context = pu_ctx;

	do
	{
		bl_new->block_next = stack_entry.load();
	} while (!stack_entry.compare_exchange_weak(bl_new->block_next,
												bl_new	      	  ,
												std::memory_order_release,
												std::memory_order_relaxed));
}

template <typename T>
T& synapse::lockfree::stack<T>::pop()
{
	block<T>* po_res;
	do
	{
		do
		{
			    po_res = stack_entry.load();
		} while(po_res == nullptr);

	} while (!stack_entry.compare_exchange_weak(po_res		      , 
												po_res->block_next,
												std::memory_order_release,
												std::memory_order_relaxed));

	return po_res->block_context;
}
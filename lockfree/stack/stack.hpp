#pragma once
#include <synapse/synapse.hpp>

#include <atomic>
#include <utility>

namespace synapse
{
namespace lockfree
{
	template <typename T>
	struct block 
	{ 
		T 		  block_context;
		block<T>* block_next   ;
	};

	template <typename T>
	class stack
	{
	public:
		stack(size_t st_count = 0);

	public:
		void	  push (T&  pu_ctx);
		void	  push (T&& pu_ctx);
		block<T>* pop  ()		   ;
		size_t    count() 		   { return stack_count.load(); }

	private:
		std::atomic<block<T>*> stack_entry;
		std::atomic<size_t>	   stack_count;
	};
}
}

template <typename T>
synapse::lockfree::stack<T>::stack(size_t st_count) 
	: stack_entry(nullptr) ,
	  stack_count(st_count) 
{
	if(st_count != 0)
	{
		block<T>* stack_entry_ptr = new block<T>[st_count];
		
		for(size_t it = 0 ; it < st_count - 1 ; it++)
			stack_entry_ptr[it].block_next = &stack_entry_ptr[it + 1];
		
		stack_entry = stack_entry_ptr;
	}
}

template <typename T>
void synapse::lockfree::stack<T>::push(T& pu_ctx)
{
	block<T>* bl_new 	  = new block<T>;
	bl_new->block_context = pu_ctx;

	do
	{
		bl_new->block_next = stack_entry.load();
	} while (!stack_entry.compare_exchange_weak(bl_new->block_next,
												bl_new	      	  ,
												std::memory_order_release,
												std::memory_order_relaxed));
	
	stack_count++;
}

template <typename T>
void synapse::lockfree::stack<T>::push(T&& pu_ctx)
{
	block<T>* bl_new 	  = new block<T>;
	bl_new->block_context = pu_ctx;

	do
	{
		bl_new->block_next = stack_entry.load();
	} while (!stack_entry.compare_exchange_weak(bl_new->block_next,
												bl_new	      	  ,
												std::memory_order_release,
												std::memory_order_relaxed));
	
	stack_count++;
}

template <typename T>
synapse::lockfree::block<T>* synapse::lockfree::stack<T>::pop()
{
	block<T>* po_res;
	do
	{
		po_res      = stack_entry.load();
		if (po_res == nullptr) return nullptr;

	} while (!stack_entry.compare_exchange_weak(po_res		      , 
												po_res->block_next,
												std::memory_order_release,
												std::memory_order_relaxed));
	
	stack_count--;
	return po_res;
}
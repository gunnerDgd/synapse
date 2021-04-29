#pragma once
#include <synapse/synapse.hpp>

#include <atomic>
#include <utility>

namespace synapse
{
namespace lockfree
{
	template <typename T>
	class stack
	{
	public:
		using block = std::pair<T, std::atomic<void*>>;
		stack() 
			: stack_entry(nullptr),
			  stack_count(0)	  { }

	public:
		void	  							push (T&  pu_ctx);
		void	  							push (T&& pu_ctx);
		synapse::lockfree::stack<T>::block* pop  ()			 ;
		size_t								count() 		 { return stack_count.load(); }

	private:
		std::atomic<block*> stack_entry;
		std::atomic<size_t>	stack_count;
	};
}
}

template <typename T>
void synapse::lockfree::stack<T>::push(T& pu_ctx)
{
	stack<T>::block* bl_new = new stack<T>::block;
	bl_new->first 		    = pu_ctx;

	do
	{
		bl_new->second = stack_entry.load();
	} while (!stack_entry.compare_exchange_weak(bl_new->second,
												bl_new	      ,
												std::memory_order_release,
												std::memory_order_relaxed));
	
	stack_count++;
}

template <typename T>
void synapse::lockfree::stack<T>::push (T&& pu_ctx)
{
	stack<T>::block* bl_new = new stack<T>::block;
	bl_new->first 		    = pu_ctx;

	do
	{
		bl_new->second = stack_entry.load();
	} while (!stack_entry.compare_exchange_weak(bl_new->second,
												bl_new	      ,
												std::memory_order_release,
												std::memory_order_relaxed));
	
	stack_count++;
}

template <typename T>
typename synapse::lockfree::stack<T>::block* synapse::lockfree::stack<T>::pop()
{
	stack<T>::block* po_res;
	do
	{
		po_res      = stack_entry.load();
		if (po_res == nullptr) return nullptr;

	} while (!stack_entry.compare_exchange_weak(po_res		  , 
												po_res->second,
												std::memory_order_release,
												std::memory_order_relaxed));
	
	stack_count--;
	return po_res;
}
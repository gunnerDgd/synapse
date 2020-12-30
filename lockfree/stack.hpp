#pragma once
#include <synapse/synapse.hpp>

#include <iostream>
#include <utility>
#include <atomic>

namespace lockfree
{
	template <typename T>
	class stack_block
	{
	public:
		T& get() { return block_context; }
		
		template <typename _sT>
		friend class stack;

	private:
		T				block_context;
		stack_block<T>* block_next = nullptr;
	};

	template <typename T>
	class stack
	{
	public:
		void push		   (T& _ctx);
		stack_block<T>* pop();

	private:
		std::atomic<stack_block<T>*> stack_entry;
	};

	template <typename T>
	void			stack<T>::push(T& _ctx)
	{
		stack_block<T>* _new = new stack_block<T>;
		_new->get()			 = _ctx;
		
		do
		{
			_new->block_next = stack_entry.load();
		} while (!stack_entry.compare_exchange_weak(_new->block_next, _new,
													std::memory_order_release,
													std::memory_order_relaxed));
	}

	template <typename T>
	stack_block<T>* stack<T>::pop()
	{
		stack_block<T>* _res;

		do
		{
			_res = stack_entry.load();
			if (_res == nullptr) return nullptr;

		} while (!stack_entry.compare_exchange_weak(_res, _res->block_next,
													std::memory_order_release,
													std::memory_order_relaxed));
		return _res;
	}
}
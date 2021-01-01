#pragma once
#include <synapse/synapse.hpp>

#include <iostream>
#include <atomic>

namespace lockfree
{
	template <class T>
	class queue_block
	{
	public:
		T& get() { return block_context; }

	private:
		T				block_context;
		queue_block<T>* block_next = nullptr;
	};

	template <typename T>
	class queue
	{
	public:
		void			enqueue(T& _ctx);
		queue_block<T>* dequeue();
		
	private:
		std::atomic<queue_block<T>*> queue_start,
									 queue_end;
	};
	
	template <typename T>
	void			queue<T>::enqueue(T& _ctx)
	{

	}

	template <typename T>
	queue_block<T>* queue<T>::dequeue()
	{

	}
}
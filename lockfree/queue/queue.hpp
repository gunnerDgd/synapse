#pragma once
#include <synapse/synapse.hpp>
#include <synapse/lockfree/block.hpp>

namespace synapse  {
namespace lockfree {

	template <typename T, size_t N = 256>
	class queue
	{
	public:
		queue ();
		
		bool 	  enqueue(T&  context);
		bool      enqueue(T&& context);
		
		block<T>* dequeue();
		
	private:
		synapse::lockfree::block<T> queue_block[N];
		std::atomic<block<T>*>      queue_read 	  ,
								    queue_write	  ;
	};

	template <typename T>
	class queue<T, 1> { };

	template <typename T>
	class queue<T, 2> { };
}
}

template <typename T, size_t N>
synapse::lockfree::queue<T, N>::queue ()
{
	for(size_t it = 0 ; it < N - 1 ; it++)
		queue_block[it].block_next = &queue_block[it + 1];
	queue_block[N - 1].block_next  = queue_block;
	
	queue_write 				   = &queue_block[0];
	queue_read  				   = &queue_block[0];
}

template <typename T, size_t N>
bool synapse::lockfree::queue<T, N>::enqueue(T& context)
{
	block<T>* enq_ptr = queue_write.load();
	do	
	{
		enq_ptr   = queue_write.load();
		if(enq_ptr->block_next == queue_read.load())
			return false;

	} while(!queue_write.compare_exchange_weak(enq_ptr					,
										       enq_ptr->block_next		,
										       std::memory_order_release,
										       std::memory_order_relaxed
										      ));
	enq_ptr->block_context = context;
	return   true;
}

template <typename T, size_t N>
bool synapse::lockfree::queue<T, N>::enqueue(T&& context)
{
	block<T>* enq_ptr = queue_write.load();
	do	
	{
		enq_ptr   = queue_write.load();
		if(enq_ptr->block_next == queue_read.load())
			return false;

	} while(!queue_write.compare_exchange_weak(enq_ptr					,
										       enq_ptr->block_next		,
										       std::memory_order_release,
										       std::memory_order_relaxed
										      ));
	enq_ptr->block_context = context;
	return   true;
}

template <typename T, size_t N>
synapse::lockfree::block<T>* synapse::lockfree::queue<T, N>::dequeue()
{
	block<T>* deq_ptr;
	do
	{
		deq_ptr 	   		    = queue_read .load();
		if(deq_ptr->block_next == queue_write.load() ||
		   deq_ptr			   == queue_write.load())
			return nullptr;
		
	} while(!queue_read.compare_exchange_weak(deq_ptr,	
										   	  deq_ptr->block_next,
										      std::memory_order_release,
										      std::memory_order_relaxed
										     ));
	
	return deq_ptr;
}
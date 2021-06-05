#pragma once
#include <synapse/synapse.hpp>
#include <synapse/lockfree/block.hpp>
#include <synapse/sync/event/event.hpp>

#include <xmmintrin.h>

namespace synapse  {
namespace lockfree {

	enum  queue_state { normal, error };

	template <typename T, size_t N = 256>
	class queue
	{
	public:
		queue (synapse::memory::memory& q_mem);

	public:
		bool 	    enqueue  (T&  context);
		bool        enqueue  (T&& context);
		
		T* 		    dequeue  ();
		
	private:
		synapse::lockfree::block<T> queue_block[N];
		
		synapse::memory::memory&    queue_memory  		;
		void*						queue_memory_pointer;
		
	private:
		std::atomic<block<T>*>      queue_read 	    ,
								    queue_write	    ;
		queue_state					queue_state_flag;

	};

	template <typename T>
	class queue<T, 1> { };

	template <typename T>
	class queue<T, 2> { };
}
}

template <typename T, size_t N>
synapse::lockfree::queue<T, N>::queue (synapse::memory::memory& q_mem)
	: queue_memory        (q_mem),
	  queue_memory_pointer(q_mem.memory_pointer()),
	  queue_state_flag	  (synapse::lockfree::queue_state::normal)
{
	std::cout << "Test !";
	if(queue_memory.state() != synapse::memory::memory_state::normal)
	{
		queue_state_flag = synapse::lockfree::queue_state::error;
		return;
	}

	if(queue_memory.memory_size() < sizeof(T) * N)
	{
		queue_state_flag = synapse::lockfree::queue_state::error;
		return;
	}

	for(size_t it = 0 ; it < N - 1 ; it++)
	{
		queue_block[it].block_context = &reinterpret_cast<T*>(queue_memory_pointer)[it];
		queue_block[it].block_next    = &queue_block[it + 1];
	}

	queue_block[N - 1].block_context  = &reinterpret_cast<T*>(queue_memory_pointer)[N - 1];
	queue_block[N - 1].block_next     = queue_block;
	
	queue_write 				     = &queue_block[1];
	queue_read  				     = &queue_block[0];
}

template <typename T, size_t N>
bool synapse::lockfree::queue<T, N>::enqueue(T& context)
{
	block<T>* enq_ptr;
	do	
	{
		if((enq_ptr = queue_write.load()) == queue_read.load())
			return false;

	} while(!queue_write.compare_exchange_weak(enq_ptr					,
										       enq_ptr->block_next		,
										       std::memory_order_release,
										       std::memory_order_relaxed
										      ));
	*enq_ptr->block_context = context;
	return   true;
}

template <typename T, size_t N>
bool synapse::lockfree::queue<T, N>::enqueue(T&& context)
{
	block<T>* enq_ptr;
	do	
	{
		if((enq_ptr = queue_write.load()) == queue_read.load())
			return false;

	} while(!queue_write.compare_exchange_weak(enq_ptr					,
										       enq_ptr->block_next		,
										       std::memory_order_release,
										       std::memory_order_relaxed
										      ));
	*enq_ptr->block_context = context;
	return   true;
}

template <typename T, size_t N>
T* synapse::lockfree::queue<T, N>::dequeue()
{
	block<T>* deq_ptr;
	do
	{
		if(deq_ptr = queue_read.load(); deq_ptr->block_next == queue_write.load())
			return nullptr;
		
	} while(!queue_read.compare_exchange_weak(deq_ptr,	
										   	  deq_ptr->block_next,
										      std::memory_order_release,
										      std::memory_order_relaxed
										     ));
	
	return deq_ptr->block_context;
}
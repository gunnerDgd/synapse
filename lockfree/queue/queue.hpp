#pragma once
#include <synapse/synapse.hpp>
#include <synapse/lockfree/block.hpp>
#include <synapse/sync/event/event.hpp>

#include <xmmintrin.h>

namespace synapse  {
namespace lockfree {

	template <typename T, size_t N = 256>
	class queue
	{
	public:
		queue				 ();
		void 	    enqueue  (T&  context);
		void        enqueue  (T&& context);
		
		T& 		    dequeue  ();
		
	private:
		synapse::lockfree::block<T> queue_block[N];

		std::atomic<block<T>*>      queue_read 	    ,
								    queue_write	    ;

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
		queue_block[it].block_next    = &queue_block[it + 1];
	queue_block[N - 1].block_next     = queue_block			;
	
	queue_write 				     = &queue_block[1];
	queue_read  				     = &queue_block[0];
}

template <typename T, size_t N>
void synapse::lockfree::queue<T, N>::enqueue(T& context)
{
	block<T>* enq_ptr;
	do	
	{
		if((enq_ptr = queue_write.load()) == queue_read.load()) {
			_mm_pause();
			continue   ;
		}

	} while(!queue_write.compare_exchange_weak(enq_ptr					,
										       enq_ptr->block_next		,
										       std::memory_order_release,
										       std::memory_order_relaxed
										      ));
	enq_ptr->block_context = context;
}

template <typename T, size_t N>
void synapse::lockfree::queue<T, N>::enqueue(T&& context)
{
	block<T>* enq_ptr;
	do	
	{
		if((enq_ptr = queue_write.load()) == queue_read.load()) {
			_mm_pause();
			continue   ;
		}

	} while(!queue_write.compare_exchange_weak(enq_ptr					,
										       enq_ptr->block_next		,
										       std::memory_order_release,
										       std::memory_order_relaxed
										      ));
	*enq_ptr->block_context = context;
}

template <typename T, size_t N>
T& synapse::lockfree::queue<T, N>::dequeue()
{
	block<T>* deq_ptr;
	do
	{
		if(deq_ptr = queue_read.load(); deq_ptr->block_next == queue_write.load()) 
		{
			_mm_pause();
			continue   ;
		}
		
	} while(!queue_read.compare_exchange_weak(deq_ptr,	
										   	  deq_ptr->block_next,
										      std::memory_order_release,
										      std::memory_order_relaxed
										     ));
	
	return deq_ptr->block_context;
}
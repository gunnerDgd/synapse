#pragma once
#include <synapse/synapse.hpp>
#include <synapse/sync/event.hpp>

#include <iostream>
#include <optional>
#include <atomic>

namespace lockfree
{
	template <typename T>
	struct cq_block
	{
		T 						  cq_context;
		std::atomic<cq_block<T>*> cq_next = nullptr;
	};
	
	template <typename T, size_t N>
	class cqueue
	{
	public:
		cqueue ();
		~cqueue();
		
		void 			 enqueue(T context);
		std::optional<T> dequeue();
		
	private:
		cq_block<T> 			  cq_entry[N];
		std::atomic<cq_block<T>*> cq_read,
								  cq_write;
		
	private:
		synchronous::event		  cq_rdy;
	};
}

template <typename T, size_t N>
lockfree::cqueue<T, N>::cqueue ()
{
	for(size_t q_it = 0 ; q_it < N - 1 ; q_it++)
		cq_entry[q_it].cq_next = &cq_entry[q_it + 1];
	
	cq_entry[N - 1].cq_next	   = cq_entry;
	cq_read					   = cq_entry;
	cq_write				   = cq_entry;
}

template <typename T, size_t N>
lockfree::cqueue<T, N>::~cqueue() {}

template <typename T, size_t N>
void lockfree::cqueue<T, N>::enqueue(T context)
{
	cq_block<T>* cq_ptr;
	do	
	{
		cq_ptr   = cq_write.load(std::memory_order_relaxed);
	} while(!cq_write.compare_exchange_weak(cq_ptr,
										    cq_ptr->cq_next,
										    std::memory_order_release,
										    std::memory_order_relaxed
										   ));
	cq_ptr->cq_context = context;
}

template <typename T, size_t N>
std::optional<T> lockfree::cqueue<T, N>::dequeue()
{
	cq_block<T>* cq_ptr;
	do
	{
		cq_ptr 	   			= cq_read.load(std::memory_order_relaxed);
	} while(!cq_read.compare_exchange_weak(cq_ptr,	
										   cq_ptr->cq_next,
										   std::memory_order_release,
										   std::memory_order_relaxed
										  ));
	
	return cq_ptr->cq_context;
}
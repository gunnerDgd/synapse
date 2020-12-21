#pragma once
#include <iostream>
#include <synapse/synapse.hpp>

namespace memory
{
	template <typename T>
	class memory_object
	{
	public:
		template <typename V>
		virtual memory_object<V> view()	   = 0;
		
		size_t			 size()			  { return memory_size; }
		virtual T& operator[](size_t _off) = 0;


	protected:
		size_t     memory_size; 
	};

	class memory_pointer_trait
	{
	public:
		void*	   native_pointer() { return memory_pointer; }

	protected:
		void*	   memory_pointer;
	};
}
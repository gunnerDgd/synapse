#pragma once
#include <iostream>
#include <functional>

#include <synapse/synapse.hpp>

namespace memory
{
	enum class memory_trait
	{
		pointer,
		non_pointer
	};
	
	class memory_object
	{
    public:
		memory_object(size_t _memsize) : memory_object_size(_memsize) {}
		  
	protected:
		virtual void allocate  () = 0;
		virtual void deallocate() = 0;
	  
	public:
		size_t 		 size	   () 	 { return memory_object_size; }

	protected:
		size_t 		 memory_object_size;
		memory_trait memory_object_trait;
	};
}
#pragma once
#include <iostream>
#include <functional>

#include <synapse/synapse.hpp>

namespace memory
{
	enum class trait { pointer, non_pointer };
	enum	   permission 
	{
		allocate   = (1 << 0),
		deallocate = (1 << 1),
		
		read	   = (1 << 2),
		write	   = (1 << 3)
	}
	
	
	class memory
	{
    public:
		memory(size_t _memsize) : memory_object_size(_memsize) {}
		  
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
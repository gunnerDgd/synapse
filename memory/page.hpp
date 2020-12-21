#pragma once
#include <synapse/memory/memory_object.hpp>

namespace memory
{
	template <typename T>
	class page_object : public memory::memory_object<T>, 
						public memory::memory_pointer_trait
	{
	public:
		
	};
}
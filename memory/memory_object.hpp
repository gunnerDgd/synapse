#pragma once
#include <iostream>
#include <synapse/synapse.hpp>
#include <synapse/memory/view.hpp>

namespace memory
{
	template <typename T>
	class memory_object
	{
	public:
		memory_object(size_t _len, size_t _size) 
			: memory_size	   (_size),
			  memory_array_size(_len)	{}

		size_t			 size()			  { return memory_size; }
		virtual T& operator[](size_t _off) = 0;


	protected:
		size_t     memory_size,
				   memory_array_size;
	};

	template <typename T>
	class memory_pointer_trait : public memory::memory_object<T>
	{
	public:
		memory_pointer_trait(void* _ptr, size_t _len, size_t _size)
			: memory_pointer		  (_ptr),
			  memory::memory_object<T>(_len, _size)
		{}
		
		void*	   native_pointer() { return memory_pointer; }

	public:
		template <typename V>
		memory::view<V> view(size_t _off, size_t _len)
		{
			if (_len * sizeof(V) + _off >= memory_size) return memory::view<V>();
			else										return memory::view<V>(memory_pointer, _off, _len); 
		}

	protected:
		void*	   memory_pointer;
	};
}
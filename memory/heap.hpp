#pragma once
#include <synapse/memory/memory_object.hpp>

namespace memory
{
	template <typename T>
	class heap_object : public memory::memory_pointer_trait<T>
	{
	public:
		heap_object(uint64_t _len)
			: memory::memory_pointer_trait<T>(nullptr, _len, _len * sizeof(T))
		{
			memory_pointer = new T[_len];
		}

		~heap_object()
		{
			delete[] memory_pointer;
		}

		T& operator[](size_t _off)
		{
			return reinterpret_cast<T*>(memory_pointer)[_off % memory_array_size];
		}
	};
}
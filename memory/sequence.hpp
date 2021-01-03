#pragma once
#include <iostream>
#include <vector>

#include <synapse/include/stream.hpp>
#include <synapse/lockfree/stack.hpp>

namespace memory
{
	// Thread - Safe Memory Sequence.
	class sequence
	{
	public:
		template <typename T>
		sequence& operator& (T& _add)	  { sequence_vec.push(std::make_pair(&_add, sizeof(T))); }

		template <typename T, size_t N>
		sequence& operator& (T(&_add)[N]) { sequence_vec.push(std::make_pair(_add, sizeof(T) * N)); }

	private:
		lockfree::stack<std::pair<void*, size_t>> sequence_vec;
	};
}
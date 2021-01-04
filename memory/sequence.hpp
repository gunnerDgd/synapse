#pragma once
#include <iostream>
#include <vector>

#include <synapse/memory/memory_object/trait/pointer_trait.hpp>
#include <synapse/lockfree/stack.hpp>

namespace memory
{
	// Thread - Safe Memory Sequence.
	class sequence
	{
	public:
		template <typename T>
		sequence& operator& (memory::pointer_trait& _add) { sequence_vec.push(_add); }

	private:
		lockfree::stack<memory::pointer_trait> sequence_vec;
	};
}
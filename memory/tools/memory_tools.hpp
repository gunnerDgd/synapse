#include <iostream>

#include <synapse/memory/memory_object/trait/pointer_trait.hpp>
#include <synapse/memory/memory_object/type/access.hpp>
#include <synapse/memory/memory_object/type/view.hpp>

namespace memory
{
    template <typename T>
	std::vector<std::access<T>> find();
}
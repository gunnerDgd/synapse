#pragma once
#include <iostream>
#include <utility>
#include <atomic>

#include <synapse/memory/vmem/vmem.hpp>

namespace synapse  {
namespace lockfree {

	template <typename T>
	struct block
	{
		T		* block_context;
		block<T>* block_next   ;
	};
}
}
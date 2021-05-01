#pragma once
#include <iostream>
#include <utility>
#include <atomic>

namespace synapse  {
namespace lockfree {

	template <typename T>
	struct block 
	{ 
		T 		  block_context;
		block<T>* block_next   ;
	};
}
}

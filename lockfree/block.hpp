#pragma once
#include <iostream>
#include <utility>
#include <atomic>


namespace synapse  {
namespace lockfree {

	template <typename T>
	class block
	{
	public:
		block(T&  ctx) : block_context(ctx) { }
		block(T&& ctx) : block_context(ctx) { }
		block()		    					{ }

	public:
		T		  block_context;
		block<T>* block_next   ;
	};
}
}
#pragma once
#include <iostream>
#include <map>

#include <synapse/memory/memory_object/trait/pointer_trait.hpp>
#include <synapse/memory/memory_object/type/access.hpp>

namespace memory
{	
	class  segment
	{
	public:
		uint8_t&		 operator[] (size_t offset);

		void add_segment   (memory::access<uint8_t>*);
		void delete_segment(size_t offset);

	private:
		std::map<size_t, memory::access<uint8_t>*> segment_map;
	};
}
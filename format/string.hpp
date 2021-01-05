#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <set>

#include <synapse/memory/memory_object/type/view.hpp>
#include <synapse/memory/memory_object/type/heap.hpp>
#include <synapse/memory/segment/segment.hpp>

namespace format
{
	class str : public memory::segment
	{
	public:
		str(const char*& _cstr);



		std::vector<memory::view<char>> split	   (str& delim);
		void							trim 	   (str& delim);
		std::set<size_t>				find	   (str& delim);
		
		void							operator+  (str& _add_str);
		void							operator+= (str& _add_str);
		void							operator=  (str& _cp_str) ;
	};
}
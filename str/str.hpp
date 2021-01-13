#include <iostream>
#include <vector>
#include <set>

#include <synapse/memory/memory_object/type/access.hpp>

namespace str
{
	using str_view 		= memory::access<char>;
	using str_view_list = std::vector<str_view>;
	
	class str
	{
	public:
		str							   (const char* _str, size_t _size);

		str& 				operator+  (str& _ctx);
		str& 				operator+= (str& _ctx);
		str& 				operator=  (str& _ctx);
		
		str_view_list 		split	   (str& delim);
		std::set<size_t>    find 	   (str& delim);
		void  				trim 	   (str& delim);
		
	private:
		char*  str_ptr;
		size_t str_size;
	};
}
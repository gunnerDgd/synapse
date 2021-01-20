#include <iostream>
#include <variant>
#include <synapse/string/string_tools.hpp>

namespace http
{	
	class header
	{
	public:
		header(std::string_view& hv);
		
	public:
		std::variant<std::string_view, std::string> prefix,
						 							context;
	};
}

http::header::header(std::string_view& hv)
{
	size_t hv_border  = hv.find(':');
	if	  (hv_border == std::string::npos) return;
	
	prefix 		      = hv.substr(0, hv_border);
	context		  	  = hv.substr(hv_border + 1, hv.length() - prefix.length() - 1);
}
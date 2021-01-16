#include <synapse/string/string_tools.hpp>

namespace http
{
	class request_prefix
	{
	public:
		request_prefix(std::string_view& rv);
		
	public:
		std::string_view method,
						 url,
						 version;
	};
	
	class response_prefix
	{
	public:
		response_prefix(std::string_view& rv);
		
	public:
		std::string_view version,
						 code,
						 message;
	};
}

http::request_prefix::request_prefix(std::string_view& rv)
{
	std::vector<std::string_view> rv_col = string::split(rv, " ");
	if(rv_col.size() != 3)		  return; // Malformed.
	
	method  = rv_col[0];
	url	    = rv_col[1];
	version = rv_col[2];
}

http::response_prefix::response_prefix(std::string_view& rv)
{
	std::vector<std::string_view> rv_col = string::split(rv, " ");
	if(rv_col.size() != 3)		  return; // Malformed.
	
	version = rv_col[0];
	code	= rv_col[1];
	message = rv_col[2];
}
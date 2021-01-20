#include <synapse/string/string_tools.hpp>

namespace http
{
	class request_prefix
	{
	public:
		request_prefix(std::string_view& rv) { parse(rv); }
		request_prefix() {}
		
		void parse    (std::string_view& rv);
		
	public:
		std::variant<std::string_view, std::string> method,
						 							url,
						 							version;
	};
	
	class response_prefix
	{
	public:
		response_prefix(std::string_view& rv) { parse(rv); }
		response_prefix() {}
		
		void parse	   (std::string_view& rv);
		
	public:
		std::variant<std::string_view, std::string> version,
						 							code,
						 							message;
	};
}

void http::request_prefix::parse    (std::string_view& rv)
{
	std::vector<std::string_view> rv_col = string::split(rv, " ");
	if(rv_col.size() != 3)		  return; // Malformed.
	
	method  = rv_col[0];
	url	    = rv_col[1];
	version = rv_col[2];
}

void http::response_prefix::parse   (std::string_view& rv)
{
	size_t rv_ver  = rv.find(' ', 0),
		   rv_code = rv.find(' ', rv_ver + 1);
	
	if(rv_ver 	 == std::string_view::npos
	  || rv_code == std::string_view::npos) return;
	
	version = rv.substr(0, rv_ver);
	code	= rv.substr(rv_ver  + 1, rv_code - rv_ver - 1);
	message = rv.substr(rv_code + 1, rv.length() - rv_code - 1);
}
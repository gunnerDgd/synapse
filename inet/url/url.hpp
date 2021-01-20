#include <iostream>
#include <synapse/string/string_tools.hpp>

namespace url
{
	class url
	{
	public:
		url				   (const char* name);
		url				   () {}
		std::string get_url();
		
	public:
		std::string url_protocol,
					url_hostname,
					url_port;
	};
}

url::url				   (const char* name)
{
	
}

std::string get_url();
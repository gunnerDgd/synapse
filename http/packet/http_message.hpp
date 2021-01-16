#include <synapse/http/packet/http_header.hpp>
#include <synapse/http/packet/http_prefix.hpp>

namespace http
{
	class request_message
	{
	public:
		request_message (char* msg, size_t msg_sz);
		~request_message() 						  { delete rprefix; }
		
	public:
		request_prefix	   *rprefix;
		std::vector<header> rheader;
		
	public:
		uint8_t*			context;
		size_t				context_size;
	};
	
	class response_message
	{
	public:
		response_message (char* msg, size_t msg_sz);
		~response_message() 					   { delete rprefix; }
		
	public:
		response_prefix	   *rprefix;
		std::vector<header> rheader;
		
	public:
		uint8_t*			context;
		size_t				context_size;
	};
}

http::request_message::request_message(char* msg, size_t msg_sz)
{
	context 	   = (uint8_t*)strstr(msg, "\r\n\r\n") + 0x04;
	size_t mborder = (size_t)context - (size_t)msg;
	
	context_size   = (context == (uint8_t*)0x04) ? 0
									   			 : msg_sz - mborder;

	std::string_view			  mv	  (msg, mborder);
	std::vector<std::string_view> mv_col = string::split(mv, "\r\n");
	
	if(mv_col.size() == 0) return; // Malformed;
	
	rprefix 	   = new request_prefix(mv_col[0]);
	for(size_t it  = 1 ; it < mv_col.size() ; it++)
		rheader.push_back(header(mv_col[it]));
}

http::response_message::response_message(char* msg, size_t msg_sz)
{
	context 	   = (uint8_t*)strstr(msg, "\r\n\r\n") + 0x04;
	size_t mborder = (size_t)context - (size_t)msg;
	
	context_size   = (context == (uint8_t*)0x04) ? 0
									   			 : msg_sz - mborder;

	std::string_view			  mv	  (msg, mborder);
	std::vector<std::string_view> mv_col = string::split(mv, "\r\n");

	if(mv_col.size() == 0) return; // Malformed;
	
	rprefix  	  = new response_prefix(mv_col[0]);
	for(size_t it = 1 ; it < mv_col.size() ; it++)
		rheader.push_back(header(mv_col[it]));
}
#include <chrono>
#include <synapse/http/packet/http_message.hpp>

#define HTTP_SAMPLE "HTTP/1,1 200 OK\r\n\
Connection : close\r\n\
Data : Thu, 03 Jul 2003 12:00:15 GMT\r\n\
Server : Apache/1.3.0 (Unix)\r\n\
Last-Modified : Sun, 5 May 2003 09:23:24 GMT\r\n\
Content-Length : 6821\r\n\
Content-Type : text/html\r\n\r\n"

int main()
{
	auto start = std::chrono::high_resolution_clock::now();
	http::response_message resp(HTTP_SAMPLE, strlen(HTTP_SAMPLE));
	auto end = std::chrono::high_resolution_clock::now();
	
	std::cout << "Parsing Delay : " << (end - start).count() << std::endl;
	
	std::cout << resp.rprefix->version << std::endl;
	std::cout << resp.rprefix->code << std::endl;
	std::cout << resp.rprefix->message << std::endl;
	
	for(auto& it : resp.rheader)
		std::cout << it.prefix << " " << it.context << std::endl;
	
	return 0;
}
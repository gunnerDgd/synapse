#include <synapse/http/socket/http_socket.hpp>
#include <synapse/http/packet/http_message.hpp>
#include <cstring>
#define TSTCHAR "GET / HTTP/1.1\r\n\r\n"

int main()
{
	network::tcp sock("125.209.222.141", 80);
	if(!sock.connect()) 
	{
		std::cout << "Error" << std::endl;
		return 2;
	}
	
	http::request_socket w(&sock);

	w.write_prefix("GET", "http://www.naver.com", "HTTP/1.1");
	w.write_header		  ("Connection", "close");
	w.write_request		  ();
	
	std::cout << "Send Complete" << std::endl;
	
	char test[4096];
	size_t rsize = sock.recv((uint8_t*)test, 4096);
	
	http::response_message resp(test, rsize);
	std::cout << resp.rprefix.version << std::endl << resp.rprefix.code << std::endl << resp.rprefix.message << std::endl;
}
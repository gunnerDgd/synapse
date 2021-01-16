#include <synapse/http/http_server.hpp>

void test_handler(network::tcp* sock, http::request_message msg)
{
	std::cout << "New Connection" << std::endl;
	char* tst = "HTTP/1.1 200 OK\r\nContent-Type : text/html\r\nContent-Length : 11\r\n\r\nHello World";
	sock->send((uint8_t*)tst, strlen(tst));
}

int main()
{
	http::server sv(6500);
	
	sv.client_handler = std::bind(test_handler, std::placeholders::_1, std::placeholders::_2);
	sv.start_server();
	
	while(true)
	{
		usleep(1000*1000);
	}
}
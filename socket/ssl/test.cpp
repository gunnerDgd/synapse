#include <synapse/inet/http/packet/http_message.hpp>
#include <synapse/socket/ssl/tls.hpp>

int main()
{
	network::tls tls_sock("172.217.31.132", 443);
	if(!tls_sock.connect()){
		std::cout << "Failed...\n";
		return 1;
	}
	
	http::message msg;
	
	msg.prefix("GET" , "/", "HTTP/1.1") ;
	msg.header("Host", "www.google.com");
	msg.header("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/ *;q=0.8");
	
	tls_sock << msg >> msg;
	
	std::cout << std::get<std::string_view>(msg.msg_prefix) << std::endl;
}
#include <synapse/socket/tcp/tcp.hpp>
#include <synapse/inet/http/packet/http_message.hpp>

int main()
{
	network::tcp  s("125.209.222.141", 80);
	s.connect();
	http::message msg;
	
	msg.write_prefix("GET", "/", "HTTP/1.1");
	msg.write_header("Connection", "keep-alive");
	
	s << msg; std::cout << "Msg Sent\n";
	s >> msg; std::cout << "Msg Recv\n";
	
	std::cout << std::get<std::string_view>(msg.msg_prefix) << std::endl;
	
	return 0;
}
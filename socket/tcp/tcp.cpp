#include "tcp.hpp"
using namespace synapse;

network::tcp::tcp(address::ipv4 a, stream::io_mode i) 
			: network::socket_base(a)
{
	if(i == stream::io_mode::block)
		open_block   ();
	else
		open_nonblock();
}


network::tcp::tcp(network::socket_type s, sockaddr_in& a, stream::io_mode i)
	: network::socket_base(s, a) {}

void   network::tcp::open_block	  () { socket_descriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); }
void   network::tcp::open_nonblock()
{
	socket_descriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	int socket_flag   = fcntl (socket_descriptor, F_GETFL, 0);
	
	fcntl(socket_descriptor, F_SETFL, socket_flag | O_NONBLOCK);
}

bool   network::tcp::connect()
{
	return (::connect(socket_fd,
			 reinterpret_cast<sockaddr*>(&socket_address),
			 sizeof(sockaddr_in)) == 0) ? true : false;
}

void   network::tcp::disconnect()
{
#ifdef UNIX_MODE
	close(socket_fd);
#else
	closesocket(socket_fd);
#endif
}

size_t network::tcp::send(uint8_t* s_ctx, size_t s_size) 
{
	if(s_size == 0 || s_ctx == nullptr) 
		return 0;
	else
		return ::send(socket_fd, (const char*)s_ctx, s_size, 0); 
}

size_t network::tcp::recv(uint8_t* r_ctx, size_t r_size) 
{
	if(r_size == 0 || r_ctx == nullptr) 
		return 0;
	else
		return ::recv(socket_fd, (char*)r_ctx, r_size, 0);
}
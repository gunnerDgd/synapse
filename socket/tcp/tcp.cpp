#include "tcp.hpp"

network::tcp::tcp(const char* _ip, unsigned short _port) 
	: network::socket_base(_ip, _port)
{
	socket_fd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}


network::tcp::tcp(network::socket_type _sock, sockaddr_in& _addr)
	: network::socket_base(_sock, _addr) {}

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
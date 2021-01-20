#include "udp.hpp"

network::udp::udp(const char* _ip, unsigned short _port)
	: socket_base(_ip, _port)
{
	socket_fd = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
}

network::udp::udp(socket_type _sock, sockaddr_in& _addr)
	: socket_base(_sock, _addr) {}

void   		 network::udp::receiver(const char* _ip, unsigned short _port)
{
	socket_address.sin_addr.s_addr = inet_addr(_ip);
	socket_address.sin_port        = htons    (_port);
}

sockaddr_in& network::udp::sender  () { return udp_last_sender; }

size_t network::udp::send(uint8_t* s_ctx, size_t s_size)
{
	if(s_ctx == nullptr || s_size == 0)
		return 0;
	else
		return ::sendto  (socket_fd,
						  (const void*)s_ctx,
						  s_size,
						  0,
						  (const sockaddr*)&socket_address,
						  sizeof(sockaddr_in)
						 );
}

size_t network::udp::recv(uint8_t* r_ctx, size_t r_size)
{
	size_t addr_len = sizeof(sockaddr_in);
	
	if(r_ctx == nullptr || r_size == 0)
		return 0;
	else
		return ::recvfrom(socket_fd,
						  (void*)r_ctx,
						  r_size,
						  0,
						  (sockaddr*) &udp_last_sender,
						  (socklen_t*)&addr_len
						 );
}
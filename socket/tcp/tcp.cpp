#include "tcp.hpp"

network::tcp::tcp(const char* _ip, unsigned short _port)
	: network::socket_base(_ip, _port) {}

network::tcp::tcp(network::socket_type _sock, sockaddr_in& _addr)
	: network::socket_base(_sock, _addr) {}

bool   network::tcp::connect()
{
	bool res = (::connect(socket_fd,
				reinterpret_cast<sockaddr*>(&socket_address),
				sizeof(sockaddr_in)) == 0) ? true : false;

	if (!res && on_error) on_error(*this, error::connect_fail);
	return                res;
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
	size_t send_res = ::send(socket_fd, (const char*)s_ctx, s_size, 0);

	if (send_res <= 0 && on_error) { on_error(*this, error::send_fail); return 0; }
	if (on_send) 				   { on_send (*this, send_res); 		return 0; }
	else                             return send_res;
}

size_t network::tcp::recv(uint8_t* r_ctx, size_t r_size)
{
	size_t recv_res = ::recv(socket_fd, (char*)r_ctx, r_size, 0);

	if (recv_res <= 0 && on_error) { on_error  (*this, error::recv_fail); return 0; }
	if (on_receive) 			   { on_receive(*this, r_ctx, recv_res);  return 0; }
	else                             return recv_res;
}
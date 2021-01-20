#pragma once
#include <synapse/socket/socket.hpp>

namespace network
{
	class udp : public socket_base
	{
	public:
		udp (const char* _ip, unsigned short _port);
        udp (socket_type _sock, sockaddr_in& _addr);
		
	public:
		bool		 bind	 () 
		{ 
		return (::bind(socket_fd, 
					  (sockaddr*)&socket_address, 
					  sizeof(sockaddr_in)) == 0) ? true : false;
		}
		void 		 receiver(const char* _ip, unsigned short _port);
		sockaddr_in& sender  ();
		
	public:
        size_t send(uint8_t* s_ctx, size_t s_size) override;
        size_t recv(uint8_t* r_ctx, size_t r_size) override;
		
	private:
		sockaddr_in udp_last_sender;
	};
}
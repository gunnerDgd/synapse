#pragma once

#include <synapse/stream/stream.hpp>
#include <functional>

#ifdef UNIX_MODE
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>
#endif

#ifdef WIN32_MODE
#include <WinSock2.h>
#endif

namespace network
{

#ifdef UNIX_MODE
          using socket_type = int;
#endif

#ifdef WIN32_MODE
          using socket_type = SOCKET;
#endif

    class   socket_base : public stream::stream
    {
        public:
            socket_base (const char* _ip, unsigned short _port)
            {
#ifdef WIN32_MODE
				WSAStartup						(MAKEWORD(2, 2), &socket_ws2data);
#endif

                socket_address.sin_addr.s_addr = inet_addr(_ip);
                socket_address.sin_port        = htons    (_port);
                socket_address.sin_family      = AF_INET;
            }

            socket_base (network::socket_type _sock, sockaddr_in& _addr) :
                socket_fd(_sock), socket_address(_addr) {}

            ~socket_base() 
			{
#ifdef UNIX_MODE
				close(socket_fd); 
#else
				closesocket(socket_fd);
#endif
			}

            virtual size_t send(uint8_t* s_ctx, size_t s_size) = 0;
            virtual size_t recv(uint8_t* r_ctx, size_t r_size) = 0;

            size_t 		   read (uint8_t* r_ctx, size_t r_size) override { return recv(r_ctx, r_size); }
            size_t 		   write(uint8_t* w_ctx, size_t w_size) override { return send(w_ctx, w_size); }

        protected:
            sockaddr_in    socket_address;
            socket_type    socket_fd;

#ifdef WIN32_MODE
			WSADATA		  socket_ws2data;
#endif

    };


}
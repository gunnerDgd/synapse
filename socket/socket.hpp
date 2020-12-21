#pragma once

#include <synapse/include/stream.hpp>
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

                socket_fd                      = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
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

            size_t read (uint8_t* r_ctx, size_t r_size) override { return recv(r_ctx, r_size); }
            size_t write(uint8_t* w_ctx, size_t w_size) override { return send(w_ctx, w_size); }

        public:
            enum error
            {
                socket_fail,
                connect_fail,
                send_fail,
                recv_fail
            };

            using recv_handler       = std::function<void(network::socket_base&, uint8_t*, size_t)>;
            using send_handler       = std::function<void(network::socket_base&, size_t)>;
            using connection_handler = std::function<void(network::socket_base&)>;
            using error_handler      = std::function<void(network::socket_base&, network::socket_base::error)>;

            connection_handler on_connected,
                               on_disconnected;
            
            recv_handler       on_receive;
            send_handler       on_send;

            error_handler      on_error;

        protected:
            sockaddr_in   socket_address;
            socket_type   socket_fd;

#ifdef WIN32_MODE
			WSADATA		  socket_ws2data;
#endif

    };


}
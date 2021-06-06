#pragma once

#include <synapse/io/io.hpp>
#include <synapse/inet/address/ip/ipv4.hpp>

namespace synapse {
namespace network {
    
#ifdef ENV_UNIX
    using socket_handle = int;
#else
    using socket_handle = SOCKET;
#endif

    class socket_base : public synapse::io::io
    {
        public:
            socket_base (synapse::network::ip::v4 sock_addr) ;
            socket_base (network::socket_handle   sock_handle,
                         synapse::network::ip::v4 sock_addr) ;
                : socket_descriptor(sock_handle),
                  socket_address   (sock_addr)  { }

            ~socket_base();

            virtual size_t                  send (uint8_t* s_ctx, size_t s_size) = 0;
            virtual size_t                  recv (uint8_t* r_ctx, size_t r_size) = 0;

            size_t 		                    read (uint8_t* r_ctx, size_t r_size) override { return recv(r_ctx, r_size); }
            size_t 		                    write(uint8_t* w_ctx, size_t w_size) override { return send(w_ctx, w_size); }

            const synapse::network::ip::v4& address() { return socket_address; }

        protected:
            synapse::network::ip::v4 socket_address   ;
            socket_handle            socket_descriptor;

#ifdef WIN32_MODE
			WSADATA		             socket_ws2data;
#endif

    };
}
}

network::socket_base::socket_base   (address::ipv4& sock_addr)
                    : socket_address(sock_addr)
{
#ifdef WIN32_MODE
    WSAStartup(MAKEWORD(2, 2), &socket_ws2data);
#endif
}

network::socket_base::~socket_base()
{
#ifdef UNIX_MODE
	close      (socket_descriptor);
#else
	closesocket(socket_descriptor);
#endif
}
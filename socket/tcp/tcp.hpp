#pragma once
#include <synapse/socket/socket.hpp>
#include <synapse/inet/addr/addr.hpp>

namespace synapse
{
namespace network
{
    class tcp : public socket_base
    {
        public:
            tcp (address::ipv4 a, stream::io_mode i = stream::io_mode::block);
            tcp (socket_type   s, sockaddr_in&    a, stream::io_mode i = stream::io_mode::block);

            bool connect   ();
            void disconnect();

        private:
            void open_block   (); 
            void open_nonblock();

        public:
            size_t send(uint8_t* s_ctx, size_t s_size) override;
            size_t recv(uint8_t* r_ctx, size_t r_size) override;
    };
}
}
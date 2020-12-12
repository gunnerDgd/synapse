#include <synapse/socket/socket.hpp>

namespace network
{
    class tcp : public socket_base
    {
        public:
            tcp (const char* _ip, unsigned short _port);
            tcp (socket_type _sock, sockaddr_in& _addr);
            
            ~tcp();

            bool connect   ();
            void disconnect();

        public:
            size_t send(uint8_t* s_ctx, size_t s_size) override;
            size_t recv(uint8_t* r_ctx, size_t r_size) override;
    };
}
#include <synapse/socket/socket.hpp>

#ifdef UNIX_MODE

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <fcntl.h>
#include <unistd.h>

#endif

#ifdef WIN32_MODE

#include <Windows.h>
#include <WinSock2.h>

#endif

namespace network
{
        class tcp : public network::socket
        {
        public:
                tcp() {}

                tcp  (std::string& tcp_ip, uint16_t tcp_port);
                tcp  (socket_type _sock, struct sockaddr_in _addr) : sock_ctx(_sock), sock_address(_addr) {}

                tcp (network::tcp&& m_tcp) // Move Constructor
                {
                        sock_address = m_tcp.sock_address;
                        sock_ctx     = m_tcp.sock_ctx;
                }

                ~tcp();

                bool   connect();
                void   disconnect();

                size_t send(uint8_t* s_ctx, size_t s_size) override;
                size_t recv(uint8_t* r_ctx, size_t r_size) override;

        public:
                std::function<void(network::tcp&)> on_connected,
                                                   on_disconnected;
                
                std::function<void(network::tcp&, size_t)>           on_send;
                std::function<void(network::tcp&, uint8_t*, size_t)> on_recv;
                

        public:
                void               set_address(std::string& _ip, uint16_t _port);
                struct sockaddr_in get_address() { return sock_address; }

        private:
                struct sockaddr_in   sock_address;
                socket_type          sock_ctx;
        };
}
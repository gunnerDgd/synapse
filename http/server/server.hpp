#include <synapse/socket/tcp/tcp_server.hpp>
#include <synapse/thread/pool/thread_pool.hpp>

#include <synapse/http/packet/packet.hpp>
#define  HTTP_BUFSIZE 4096

namespace http
{
    class http_server : public network::tcp_server
    {
        public:
            http_server   (uint16_t _port, std::string _path);

            using client_handler = std::function<void(network::tcp*, packet)>;
            client_handler         on_client;

        private:
            uint16_t    http_port;
            std::string http_path;
    };
}
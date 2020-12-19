#include <synapse/socket/tcp/server/tcp_server.hpp>

#include <synapse/http/packet/request.hpp>
#include <synapse/http/packet/response.hpp>
#define  HTTP_BUFSIZE 4096

namespace http
{
    class http_server : public network::tcp_server
    {
        public:
            http_server   (uint16_t _port, std::string _path);

            using client_handler = std::function<void(network::tcp*, request*)>;
            client_handler         on_http_client;

        private:
            uint16_t    http_port;
            std::string http_path;

            void        process_request(network::tcp* _cl);
    };
}
#include <synapse/socket/tcp/tcp_server.hpp>

namespace http
{
    template <unsigned short http_port = 80>
    class server
    {
        public:
            server();

            void start();
            void end  ();

        public:
            std::function<void(network::tcp)> on_connection;

        private:
            network::tcp_server http_server("0.0.0.0", 80);
    }
}
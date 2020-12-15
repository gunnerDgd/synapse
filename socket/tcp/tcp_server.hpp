#include <synapse/socket/tcp/tcp.hpp>
#include <thread>

namespace network
{
    class tcp_server
    {
        public:
            tcp_server (const char* _ip, unsigned short _port);
            ~tcp_server() { end_server(); }

        public:
            bool start_server();
            void end_server  ();

        public:
            enum error
            {
                server_started,
                server_ended,

                socket_error,
                bind_error,
                listen_error
            };

            using connection_handler = std::function<void(network::tcp*)>;
            using error_handler      = std::function<void(network::tcp_server*, network::tcp_server::error)>;
            using server_handler     = error_handler;

            connection_handler   on_client;
            server_handler       on_server;
            error_handler        on_error;

        private:
            sockaddr_in          server_address;
            network::socket_type server_socket;
            
            std::thread         *server_thread = nullptr;
            bool                 server_running;
    };
}
#include <synapse/socket/tcp/tcp.hpp>
#include <synapse/thread/lock/signal/signal.hpp>

#include <thread>
#include <functional>

namespace network
{
        template <uint16_t port>
        class tcp_server
        {
                public:
                        tcp_server  (std::string _ip);
                        ~tcp_server() { end(); }

                        bool start(); // Starts Listening.
                        void end  (); // Ends Listening

                public:
                        std::function<void(network::tcp)>     on_connection;

                private:
                        struct sockaddr_in   server_address;
                        network::socket_type server_sock;

                private:
                        std::thread*                          server_thread;
                        
                        lock::signal<network::tcp> server_lock;
                        bool                       server_acting = false;
        };
}
#include <synapse/socket/tcp/tcp_server.hpp>
#include <synapse/thread/pool/thread_pool.hpp>

namespace http
{
    class http_handler : public thread::pooled_launcher
    {
        public:
            http_handler(network::tcp* _sock);
            void  launch() override;

        private:
            network::tcp* h_comm;
    };

    class http_server : public network::tcp_server
    {
        public:
            http_server   (uint16_t _port);
            void on_client(network::tcp* _cl);

        public:
            thread::thread_pool<4> http_pool;
    };
}
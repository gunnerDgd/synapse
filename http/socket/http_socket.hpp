#include <synapse/http/packet/http_packet.hpp>
#include <synapse/socket/tcp/tcp.hpp>

namespace http
{
    class socket
    {
        using handler_t = std::function<void(network::tcp&, std::string)>>;

        public:
            socket(network::tcp& _sock);

            void add_handler    (std::string                                     _header,
                                 std::function<void(network::tcp&, std::string)> _handler);
            
            void delete_handler (std::string _header);

        public:
            void message_handler(network::tcp& _sock, uint8_t* _msg, size_t _recv);

        private:
            network::tcp                    *http_socket;
            
            std::map<std::string, handler_t> http_handler;
            handler_t                        on_connected;
    };
}
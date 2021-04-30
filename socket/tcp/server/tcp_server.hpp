#include <synapse/socket/tcp/tcp.hpp>
#include <thread>

namespace synapse {
namespace network {

    class tcp_server
    {
    public:
        tcp_server (synapse::network::ip::v4& srv_addr);
        tcp_server () { }

        ~tcp_server() { }

    public:
        bool bind  (synapse::network::ip::v4& bnd_addr);
        bool bind  ();

        bool listen();
		
    private:
        synapse::network::ip::v4      server_address;
        synapse::network::socket_type server_socket ;
#ifdef WIN32_MODE
			WSADATA				      server_ws2data;
#endif
    
		private:
            std::thread         			  *server_thread = nullptr;
            bool                			   server_running;
    };

}
}

synapse::network::tcp_server::tcp_server (synapse::network::ip::v4& srv_addr)
    : server_address(srv_addr)
{
    server_socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

bool synapse::network::tcp_server::bind  (synapse::network::ip::v4& bnd_addr)
{
    server_address = bnd_addr;
    this->bind()
}
bool synapse::network::tcp_server::bind  ()
{
    int bind_res = ::bind();
}
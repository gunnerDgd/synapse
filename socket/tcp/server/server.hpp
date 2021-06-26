#include <synapse/socket/tcp/tcp.hpp>

#include <atomic>
#include <thread>
#include <functional>

namespace synapse {
namespace network {

    class tcp_server
    {
    public:
        using accept_t = std::function<void(synapse::network::tcp)>;
        tcp_server (synapse::network::ip::v4 srv_addr);
        ~tcp_server() 
        { 
            this         ->close();
            server_running.store(false); 
        }

    public:
        bool bind  ();
        bool listen(uint32_t backlog = 20) { return (::listen(server_socket, backlog) < 0) ? false : true; } 

        void start(accept_t on_accept);
        void close()                  { ::close(server_socket); }
		
    private:
        std::atomic<bool>               server_running;
        
        synapse::network::ip::v4        server_address;
        synapse::network::socket_handle server_socket ;
#ifdef WIN32_MODE
			WSADATA				        server_ws2data;
#endif
    };

}
}

synapse::network::tcp_server::tcp_server    (synapse::network::ip::v4 srv_addr)
    : server_address(srv_addr),
      server_running(true)    ,
      server_socket (::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) { }

bool synapse::network::tcp_server::bind ()
{
    return (::bind(server_socket, (sockaddr*)&server_address.address_context(), sizeof(sockaddr_in)) < 0) ? false : true;
}

void synapse::network::tcp_server::start(synapse::network::tcp_server::accept_t on_accept)
{
    uint32_t         cli_addr_length;
    sockaddr_in      cli_addr;


    server_running = true;
    while(true)
    {
        synapse::network::socket_handle cli_sock = ::accept(server_socket, (sockaddr*)&cli_addr, &cli_addr_length);
        
        if(cli_sock < 0 || !server_running.load())
            break;
        
        on_accept(synapse::network::tcp(cli_sock, synapse::network::ip::v4(cli_addr)));
    }
}
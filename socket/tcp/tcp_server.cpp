#include "tcp_server.hpp"

template <uint16_t port>
network::tcp_server<port>::tcp_server(std::string _ip)
{
        server_address.sin_addr.s_addr = inet_addr(_ip.c_str());
        server_address.sin_port        = htons(port);
        server_address.sin_family      = AF_INET;

        server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        bind                (server_sock, (struct sockaddr*)&server_address, sizeof(struct sockaddr_in));
}

template <uint16_t port>
bool network::tcp_server<port>::start() // Starts Listening
{
        bool res = (listen(server_sock,  20) == 0) ? true : false;

        if(res)
        {
                server_thread = new std::thread([&]()
                {
                        int                addr_size = sizeof(struct sockaddr_in);
                        struct sockaddr_in cl_addr;

                        while(server_acting)
                        {
                                network::socket_type cl_socket = accept(server_sock, 
                                                                       (struct sockaddr*)&cl_addr, 
                                                                       (socklen_t*)&addr_size);
                                on_connection                          (network::tcp(cl_socket, cl_addr));
                        }
                }, this);
        }

        return res;
}

template <uint16_t port>
void network::tcp_server<port>::end  () // Ends Listening
{
        server_acting = false;
        server_thread->join();

        delete server_thread;
        close (server_sock);
}
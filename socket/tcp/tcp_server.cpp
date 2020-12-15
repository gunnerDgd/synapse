#include "tcp_server.hpp"

network::tcp_server::tcp_server(const char* _ip, unsigned short _port)
{
    server_address.sin_addr.s_addr = inet_addr(_ip);
    server_address.sin_port        = htons    (_port);
    server_address.sin_family      = AF_INET;

    server_socket                  = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

void network::tcp_server::end_server  ()
{
    server_running = false;
    close              (server_socket);
    
    server_thread ->join();
}

bool network::tcp_server::start_server()
{
    bool start_res = ::bind(server_socket,
                            reinterpret_cast<sockaddr*>(&server_address), 
                            sizeof(sockaddr_in));
    
    if  (!start_res && on_error) { on_error(*this, error::bind_error);   return false; }

    start_res      = ::listen(server_socket, 20);
    if  (!start_res && on_error) { on_error(*this, error::listen_error); return false; }

    server_thread = new std::thread([&, this]()
                    {
                        network::socket_type cl_socket;
                      
                        sockaddr_in          cl_address;
                        int                  cl_size = sizeof(sockaddr_in);

                        while(server_running == true)
                        {
                            cl_socket = ::accept(server_socket,
                                                 reinterpret_cast<sockaddr*>(&cl_address),
                                                (socklen_t*)&cl_size);
                                            
                            network::tcp   *cl = new network::tcp(cl_socket, cl_address);
                            this->on_client(cl);
                        }
                    });

    if(on_server)  on_server(*this, error::server_started);
    return true;
}
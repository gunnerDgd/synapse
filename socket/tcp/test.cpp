#include <iostream>
#include <synapse/socket/tcp/tcp_server.hpp>

int main()
{
    network::tcp_server t_server("0.0.0.0", 6600);
    network::tcp        t_sock("127.0.0.1", 6600);

    t_server.on_client = [](network::tcp* _cl)->void
        {
            std::cout << "Client Connected" << std::endl;
            _cl->mode(stream::stream::stream_mode::async);

            _cl->send((uint8_t*)"Hello World", 11);
        };
    t_server.on_error = [](network::tcp_server* _serv, network::tcp_server::error _err)
    {
        std::cout << (int)_err << std::endl;
    };
    
    if(!t_server.start_server()) { return 0; }

    if(!t_sock.connect()) { std::cout << "Connection Failed" << std::endl; return 0; }

    char   buf[11];
    t_sock.mode(stream::stream::stream_mode::async);

    t_sock >> buf;
    std::cout << buf << std::endl;

    t_server.end_server();
}
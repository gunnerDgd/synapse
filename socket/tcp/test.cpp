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
    t_server.start_server();

    std::cout << "Ready to Connect" << std::endl;
    if(!t_sock.connect()) { std::cout << "Connection Failed" << std::endl; }
    std::thread t_cl([&]()
    {
        char   buf[11];
        t_sock.mode(stream::stream::stream_mode::async);
        t_sock >> buf;
        std::cout << buf << std::endl;
    });

    t_cl.join();
    t_server.end_server();
}
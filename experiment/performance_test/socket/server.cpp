#include <synapse/socket/tcp/server/server.hpp>
#include <thread>

int main()
{
    synapse::network::tcp_server test_server(synapse::network::ip::v4("0.0.0.0", 4000));
    test_server.bind  ();
    test_server.listen();

    synapse::network::tcp_server::accept_t on_accept = [&](synapse::network::tcp s) 
                           {
                                uint8_t test_message[16];
                                s.recv (test_message, 16); std::cout << test_message << '\n';
                           };
    
    std::thread test_thread(&synapse::network::tcp_server::start, &test_server, on_accept);
    test_thread.join();
}
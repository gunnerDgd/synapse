#include <synapse/socket/tcp/tcp.hpp>

int main()
{
    synapse::network::tcp test_socket(synapse::network::ip::v4("127.0.0.1", 4000));
    test_socket.connect();

    test_socket.send((uint8_t*)"Hello World", 11);
}
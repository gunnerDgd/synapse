#include "addr.hpp"

int main()
{
    address::ipv4 a("192.168.0.2", 6550);
    a.ntoh();

    auto b = (sockaddr_in)a;

    for(uint8_t& c : a.ipv4_addr)
        std::cout << (int)c << " ";

    std::cout << std::endl;
}
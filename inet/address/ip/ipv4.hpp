#include <iostream>
#include <synapse/inet/address/address_attr.hpp>

namespace synapse {
namespace network {
namespace ip      {

    class v4
    {
    public:
        v4(std::string  v4_addr, uint16_t v4_port);
        v4(sockaddr_in& v4_addr);

        sockaddr_in& address_context() { return ipv4_address_context; }
        std::string  ipv4_address   ();
        uint16_t     ipv4_port      () { return ntohs(ipv4_address_context.sin_port); }

    protected:
        sockaddr_in                  ipv4_address_context;
    };
}
}
}

synapse::network::ip::v4::v4(sockaddr_in& v4_addr)
{
    ipv4_address_context.sin_family      = AF_INET;
    ipv4_address_context.sin_addr.s_addr = v4_addr.sin_addr.s_addr;
    ipv4_address_context.sin_port        = v4_addr.sin_port;
}

synapse::network::ip::v4::v4(std::string v4_addr, uint16_t v4_port)
{
    ipv4_address_context.sin_addr.s_addr = inet_addr(v4_addr.c_str());
    ipv4_address_context.sin_port        = htons(v4_port);
    ipv4_address_context.sin_family      = AF_INET;
}

std::string synapse::network::ip::v4::ipv4_address()
{
    uint8_t* v4_addr_pointer = (uint8_t*)&ipv4_address_context.sin_addr.s_addr;
    std::string addr_res     = std::to_string(v4_addr_pointer[0]) + "."
                             + std::to_string(v4_addr_pointer[1]) + "."
                             + std::to_string(v4_addr_pointer[2]) + "."
                             + std::to_string(v4_addr_pointer[3]); return addr_res;
}
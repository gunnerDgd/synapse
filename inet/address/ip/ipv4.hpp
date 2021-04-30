#include <iostream>
#include <synapse/inet/address/address_attr.hpp>

namespace synapse {
namespace network {
namespace ip      {

    class v4 : private sockaddr_in
    {
    public:
        v4(std::string v4_addr, uint16_t v4_port);
        v4(sockaddr_in v4_addr);

    public:
        void                         host_byte_order   ();
        void                         network_byte_order();
        
        synapse::network::byte_order current_byte_order() { return ipv4_byte_order; }
        operator                     sockaddr_in&      () { return reinterpret_cast<sockaddr_in&>(*this); }

    private:
        synapse::network::byte_order ipv4_byte_order;
    };
}
}
}

synapse::network::ip::v4::v4(sockaddr_in v4_addr)
{
    sin_family      = AF_INET;
    sin_addr.s_addr = v4_addr.sin_addr.s_addr;
    sin_port        = v4_port.sin_port;

    host_byte_order();
}

synapse::network::ip::v4::v4(std::string v4_addr, uint16_t v4_port)
{
    uint8_t* sin_addr_ptr = (uint8_t*)&sin_addr.s_addr;
    sscanf(v4_addr.c_str() ,
           "%u.%u.%u.%u"   , 
           &sin_addr_ptr[0],
           &sin_addr_ptr[1],
           &sin_addr_ptr[2],
           &sin_addr_ptr[3]);

    sin_port   = v4_port;
    sin_family = AF_INET;
    
    host_byte_order ();
}

void synapse::network::ip::v4::host_byte_order   ()
{
    ipv4_byte_order = synapse::network::byte_order::host_order;

    sin_addr.s_addr = ntohl(sin_addr.s_addr);
    sin_port        = ntohs(sin_port)       ;
}

void synapse::network::ip::v4::network_byte_order()
{
    ipv4_byte_order = synapse::network::byte_order::network_order;

    sin_addr.s_addr = htonl(sin_addr.s_addr);
    sin_port        = htons(sin_port)       ;
}
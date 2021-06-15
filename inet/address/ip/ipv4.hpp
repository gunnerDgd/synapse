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

    public:
        void                         host_byte_order   ();
        void                         network_byte_order();

        synapse::network::byte_order current_byte_order() { return ipv4_byte_order;      }
        sockaddr_in&                 address_context   () { return ipv4_address_context; }

    protected:
        synapse::network::byte_order ipv4_byte_order;
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

    host_byte_order();
}

synapse::network::ip::v4::v4(std::string v4_addr, uint16_t v4_port)
{
    char*  v4_addr_pointer = (char*)&ipv4_address_context.sin_addr.s_addr;
    sscanf(v4_addr.c_str()      ,
           "%hhu.%hhu.%hhu.%hhu", 
           &v4_addr_pointer[3]  ,
           &v4_addr_pointer[2]  ,
           &v4_addr_pointer[1]  ,
           &v4_addr_pointer[0]) ;

    ipv4_address_context.sin_port   = v4_port;
    ipv4_address_context.sin_family = AF_INET;
    
    network_byte_order ();
}

void synapse::network::ip::v4::host_byte_order   ()
{
    ipv4_byte_order = synapse::network::byte_order::host_order;

    ipv4_address_context.sin_addr.s_addr = ntohl(ipv4_address_context.sin_addr.s_addr);
    ipv4_address_context.sin_port        = ntohs(ipv4_address_context.sin_port)       ;
}

void synapse::network::ip::v4::network_byte_order()
{
    ipv4_byte_order = synapse::network::byte_order::network_order;

    ipv4_address_context.sin_addr.s_addr = htonl(ipv4_address_context.sin_addr.s_addr);
    ipv4_address_context.sin_port        = htons(ipv4_address_context.sin_port)       ;
}
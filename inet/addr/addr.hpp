#include <iostream>
#include <string>
#include <synapse/synapse.hpp>

#ifdef UNIX_MODE
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>
#endif

#ifdef WIN32_MODE
#include <WinSock2.h>
#endif

namespace synapse
{
namespace address
{
    enum class byte_order
    {
        host_order,
        network_order
    };
    
    class ipv4
    {
    public:
        ipv4(const char* addr, uint16_t port);
        
        byte_order current_byte_order() { return ipv4_byte_order; }
        void       network_byte_order() { htonl(ipv4_addr_uint); htons(ipv4_port); ipv4_byte_order = byte_order::network_order; }
        void       host_byte_order   () { ntohl(ipv4_addr_uint); ntohs(ipv4_port); ipv4_byte_order = byte_order::host_order;    }
        
        operator sockaddr_in();

    private:
        union
        {
            uint8_t  ipv4_addr[4]  ;
            uint32_t ipv4_addr_uint;
        };

        uint16_t     ipv4_port;
        byte_order   ipv4_byte_order = byte_order::network_order;
    };

    /*
    IPv6 Protocol Address Will be added soon.
    */
}
}

synapse::address::ipv4::ipv4(const char* addr, uint16_t port)
{
    sscanf(addr, "%hhu.%hhu.%hhu.%hhu", &ipv4_addr[0],
                                        &ipv4_addr[1],
                                        &ipv4_addr[2],
                                        &ipv4_addr[3]);

    ipv4_port      = htons(port);
    ipv4_addr_uint = htonl(ipv4_addr_uint);
}

synapse::address::ipv4::operator sockaddr_in()
{
    sockaddr_in saddr;
            
    saddr.sin_addr.s_addr = ipv4_addr_uint ;
    saddr.sin_port        = ipv4_port      ;
    saddr.sin_family      = AF_INET        ;

   return                   saddr;
}
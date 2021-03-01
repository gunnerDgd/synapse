#include <iostream>

#ifdef UNIX_MODE
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>
#endif

#ifdef WIN32_MODE
#include <WinSock2.h>
#endif

namespace address
{
    class ipv4
    {
    public:
        ipv4(const char* addr, uint16_t port)
        {
            sscanf(addr, "%hhu.%hhu.%hhu.%hhu", &ipv4_addr[0],
                                                &ipv4_addr[1],
                                                &ipv4_addr[2],
                                                &ipv4_addr[3]);

            ipv4_port = htons(port);
        }

        void hton() { htonl(ipv4_addr_uint); htons(ipv4_port); }
        void ntoh() { ntohl(ipv4_addr_uint); ntohs(ipv4_port); }

        operator sockaddr_in()
        {
            sockaddr_in saddr;
            
            saddr.sin_addr.s_addr = ipv4_addr_uint ;
            saddr.sin_port        = ipv4_port      ;
            saddr.sin_family      = AF_INET        ;

            return      saddr;
        }

    public:
        union
        {
            uint8_t  ipv4_addr[4]  ;
            uint32_t ipv4_addr_uint;
        };

        uint16_t     ipv4_port;
    };

    /*
    IPv6 Protocol Address Will be added soon.
    */
}
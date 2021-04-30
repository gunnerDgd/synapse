#include <iostream>
#include <string>
#include <synapse/synapse.hpp>

#ifdef ENV_UNIX
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>
#endif

#ifdef ENV_WINDOWS
#include <WinSock2.h>
#endif

namespace synapse {
namespace network {
    enum class byte_order
    {
        host_order,
        network_order
    };
}
}
#include <synapse/inet/dns/dns_query.hpp>
#include <synapse/socket/udp/udp.hpp>

int main()
{
    synapse::network::ip::v4 dns_addr  ("8.8.8.8", 53);
    synapse::network::udp    dns_socket(dns_addr);
    
    if(!dns_socket.connect())
    {
        std::cerr << "## Socket Disconnected !!\n";
        return 1;
    }
 
    std::vector<std::string> url = synapse::network::dns::query_url(dns_socket, "www.google.com");
    std::cout << url.size() << std::endl;
    std::cout << url[0] << std::endl;
}
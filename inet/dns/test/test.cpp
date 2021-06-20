#include <synapse/inet/dns/dns_query.hpp>
#include <synapse/socket/udp/udp.hpp>

int main()
{
    synapse::network::ip::v4 dns_addr  ("219.250.36.130", 53);
    synapse::network::udp    dns_socket(dns_addr);
    
    dns_socket.connect();
    std::vector<std::string> url = synapse::network::dns::query_url(dns_socket, "www.naver.com");
    for(auto& u_it : url)
        std::cout << u_it << std::endl;
}
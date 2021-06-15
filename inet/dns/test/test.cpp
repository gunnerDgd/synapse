#include <synapse/inet/dns/dns_query.hpp>
#include <synapse/socket/udp/udp.hpp>

int main()
{
    synapse::network::ip::v4 dns_addr  ("10.64.192.62", 53);
    synapse::network::udp    dns_socket(dns_addr);
    
    std::cerr << dns_socket           .connect   () << std::endl;
    std::cerr << "## Socket Creation Successful\n";
 
    std::vector<std::string> url = synapse::network::dns::query_url(dns_socket, "www.naver.com");
}
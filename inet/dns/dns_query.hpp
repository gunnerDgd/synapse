#include <synapse/inet/dns/dns.hpp>
#include <synapse/inet/dns/dns_query_parse.hpp>

#include <random>

namespace synapse {
namespace network {
namespace dns     {

    std::vector<std::string> query_url(synapse::network::socket_base& q_sock,
                                       std::string                    q_url);

}
}
}

std::vector<std::string> synapse::network::dns::query_url(synapse::network::socket_base& q_sock, std::string q_url)
{
    std::mt19937                          q_tid;
    synapse::network::dns::packet::flag   q_flag;
    synapse::network::dns::packet::header q_header(q_tid(), q_flag, 1);
    
    synapse::network::dns::query_vector   q_vec;
    q_vec                                .push_back(synapse::network::dns::packet::query(q_url));

    std::vector<std::string> r_vec;
    auto                     a_vec = synapse::network::dns::write_query(q_sock, q_header, q_vec, synapse::network::dns::parse::parse_ipv4);

    return r_vec;
}
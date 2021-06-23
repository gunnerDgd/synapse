#include <synapse/inet/dns/dns_io.hpp>
#include <synapse/inet/dns/parse/answer_parse.hpp>

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
    std::mt19937                          q_tid; // Random Number Generator.
    synapse::network::dns::packet::flag   q_flag = {0, };
    
    synapse::network::dns::packet::header q_header(q_tid(), q_flag, 1);
    synapse::network::dns::query_vector   q_vec                      = { new synapse::network::dns::packet::query(q_url), 1};

    std::vector<std::string> r_vec;
    synapse::network::dns::write_query(q_sock, q_header, q_vec);

    auto             a_vec = synapse::network::dns::read_answer(q_sock, synapse::network::dns::parse::parse_ipv4);
    for(uint16_t a_it = 0 ; a_it < a_vec.second ; a_it++)
        if(a_vec.first[a_it].answer_type == synapse::network::dns::dns_type::a)
            r_vec.push_back(a_vec.first[a_it].answer_resolved);

    return r_vec;
}
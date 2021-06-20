#include <synapse/inet/dns/packet/dns_packet.hpp>
#include <synapse/inet/dns/format/name_format.hpp>
#include <synapse/inet/dns/format/byte_order.hpp>

namespace synapse {
namespace network {
namespace dns     {
namespace raw     {

    size_t serialize(synapse::network::dns::packet::header& s_header, char* s_memory);
    size_t serialize(synapse::network::dns::packet::query & s_query , char* s_memory);
    size_t serialize(synapse::network::dns::packet::answer& s_answer, char* s_memory);
    
}
}
}
}

size_t synapse::network::dns::raw::serialize(synapse::network::dns::packet::header& s_header, char* s_memory)
{
    synapse::network::dns::byte_order::host_to_network(s_header);
    memcpy(s_memory, &s_header, 12);

    return 12;
}

size_t synapse::network::dns::raw::serialize(synapse::network::dns::packet::query& s_query, char* s_memory)
{
    uint16_t q_name_length;
    char*    q_name = synapse::network::dns::name_format::host_to_network(s_query.query_name, q_name_length);

    synapse::network::dns::byte_order::host_to_network(s_query);
    memcpy(s_memory, q_name, q_name_length)   ; s_memory += q_name_length;

    *(uint16_t*)s_memory = s_query.query_type ; s_memory += 2;
    *(uint16_t*)s_memory = s_query.query_class; s_memory += 2;

    return q_name_length + 4;
}

size_t synapse::network::dns::raw::serialize(synapse::network::dns::packet::answer& s_answer, char* s_memory)
{
    uint16_t a_name_length;
    char*    a_name       = synapse::network::dns::name_format::host_to_network(s_answer.answer_name, a_name_length);

    synapse::network::dns::byte_order::host_to_network(s_answer);
    memcpy  (s_memory, a_name, a_name_length)    ; s_memory += a_name_length;
    
    *(uint16_t*)s_memory = s_answer.answer_type  ; s_memory += 2;
    *(uint16_t*)s_memory = s_answer.answer_class ; s_memory += 2;
    *(uint32_t*)s_memory = s_answer.answer_ttl   ; s_memory += 4;
    *(uint16_t*)s_memory = s_answer.answer_length; s_memory += 2;

    if(s_answer.answer_data.index() == 1)
        memcpy(s_memory, std::get<1>(s_answer.answer_data), ntohs(s_answer.answer_length));
    else
        synapse::network::dns::name_format::host_to_network(std::get<0>(s_answer.answer_data), s_memory);

    return a_name_length + s_answer.answer_length + 10;
}
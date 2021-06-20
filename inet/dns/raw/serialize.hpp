#include <synapse/inet/dns/packet/dns_packet.hpp>
#include <synapse/inet/dns/format/name_format.hpp>
#include <synapse/inet/dns/format/byte_order.hpp>

namespace synapse {
namespace network {
namespace dns     {
namespace raw     {

    void serialize(synapse::network::dns::packet::header& s_header, char*& s_memory);
    void serialize(synapse::network::dns::packet::query & s_query , char*& s_memory);
    void serialize(synapse::network::dns::packet::answer& s_answer, char*& s_memory);
    
}
}
}
}

void synapse::network::dns::raw::serialize(synapse::network::dns::packet::header& s_header, char*& s_memory)
{
    synapse::network::dns::byte_order::host_to_network(s_header);
    std::memcpy                                       (s_memory, &s_header, 12); s_memory += 12;
}

void synapse::network::dns::raw::serialize(synapse::network::dns::packet::query& s_query, char*& s_memory)
{
    synapse::network::dns::name_format::host_to_network(s_query.query_name, s_memory);
    synapse::network::dns::byte_order ::host_to_network(s_query); // Byte Order Conversion.

    *(uint16_t*)s_memory = s_query.query_type ; s_memory += 2;
    *(uint16_t*)s_memory = s_query.query_class; s_memory += 2;
}

void synapse::network::dns::raw::serialize(synapse::network::dns::packet::answer& s_answer, char*& s_memory)
{
    uint16_t sname_len = synapse::network::dns::name_format::host_to_network(s_answer.answer_name, s_memory);
    synapse::network::dns::byte_order                      ::host_to_network(s_answer);
    
    *(uint16_t*)s_memory = s_answer.answer_type  ; s_memory += 2;
    *(uint16_t*)s_memory = s_answer.answer_class ; s_memory += 2;
    *(uint32_t*)s_memory = s_answer.answer_ttl   ; s_memory += 4;
    *(uint16_t*)s_memory = s_answer.answer_length; s_memory += 2;

    std::memcpy(s_memory,  s_answer.answer_data, s_answer.answer_length);
    s_memory += s_answer.answer_length;
}
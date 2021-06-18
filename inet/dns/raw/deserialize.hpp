#include <synapse/inet/dns/packet/dns_packet.hpp>

namespace synapse {
namespace network {
namespace dns     {
namespace raw     {

    void deserialize(synapse::network::dns::packet::header& h, char*& raw_ptr);
    void deserialize(synapse::network::dns::packet::query & q, char*& raw_ptr, char* packet_raw);
    void deserialize(synapse::network::dns::packet::answer& a, char*& raw_ptr, char* packet_raw);

}
}
}
}

void synapse::network::dns::raw::deserialize(synapse::network::dns::packet::header& h, char*& raw_ptr)
{
    std::memcpy                                       (&h, raw_ptr, 12); raw_ptr += 12;
    synapse::network::dns::byte_order::network_to_host(h);
}

void synapse::network::dns::raw::deserialize (synapse::network::dns::packet::query& q, char*& raw_ptr, char* packet_raw)
{
    q.query_name  = synapse::network::dns::compress::decompress_name(raw_ptr, packet_raw);
    //std::cout << q.query_name << std::endl;
    
    q.query_type  = *(uint16_t*)raw_ptr; raw_ptr += 2;
    q.query_class = *(uint16_t*)raw_ptr; raw_ptr += 2;

    synapse::network::dns::byte_order::network_to_host(q);
}

void synapse::network::dns::raw::deserialize(synapse::network::dns::packet::answer& a, char*& raw_ptr, char* packet_raw)
{
    a.answer_name   = synapse::network::dns::compress::decompress_name(raw_ptr, packet_raw);
    a.answer_type   = *(uint16_t*)raw_ptr; raw_ptr += 2;
    a.answer_class  = *(uint16_t*)raw_ptr; raw_ptr += 2;
    a.answer_ttl    = *(uint32_t*)raw_ptr; raw_ptr += 4;
    a.answer_length = *(uint16_t*)raw_ptr; raw_ptr += 2;
    a.answer_name   = synapse::network::dns::compress::decompress_name(raw_ptr, packet_raw);

    synapse::network::dns::byte_order::network_to_host(a);
}

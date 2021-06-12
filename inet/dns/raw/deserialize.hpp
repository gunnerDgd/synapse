#include <synapse/inet/dns/packet/packet.hpp>

namespace synapse {
namespace network {
namespace dns     {
namespace raw     {

    synapse::network::dns::packet::header deserialize(char*& raw_ptr);
    synapse::network::dns::packet::query  deserialize(char*& raw_ptr, char* packet_raw);
    synapse::network::dns::packet::answer deserialize(char*& raw_ptr, char* packet_raw);

}
}
}
}

synapse::network::dns::packet::header synapse::network::dns::raw::deserialize(char*& raw_ptr)
{
    synapse::network::dns::packet::header header_res;
    std::memcpy                         (&header_res, 12, raw_ptr, 12); raw_ptr += 2;

    synapse::network::dns::byte_order::network_to_host(header_res);
    return header_res;
}

synapse::network::dns::packet::query synapse::network::dns::raw::deserialize (char*& raw_ptr, char* packet_raw)
{
    synapse::network::dns::packet::query query_res;
    
    query_res.query_name  = synapse::network::dns::compress::decompress_name(raw_ptr, packet_raw);
    query_res.query_type  = *(uint16_t*)raw_ptr; raw_ptr += 2;
    query_res.query_class = *(uint16_t*)raw_ptr; raw_ptr += 2;

    return query_res;
}

synapse::network::dns::packet::answer synapse::network::dns::raw::deserialize(char*& raw_ptr, char* packet_raw)
{
    synapse::network::dns::packet::answer answer_res;
   
    answer_res.answer_name   = synapse::network::dns::compress::decompress_name(raw_ptr, packet_raw);
    answer_res.answer_type   = *(uint16_t*)raw_ptr; raw_ptr += 2;
    answer_res.answer_class  = *(uint16_t*)raw_ptr; raw_ptr += 2;
    answer_res.answer_ttl    = *(uint32_t*)raw_ptr; raw_ptr += 4;
    answer_res.answer_length = *(uint16_t*)raw_ptr; raw_ptr += 2;
    answer_res.answer_name   = synapse::network::dns::compress::decompress_name(raw_ptr, packet_raw);
 
    return answer_res;
}

#include <synapse/inet/dns/packet/packet.hpp>

namespace synapse    {
namespace network    {
namespace dns        {
namespace byte_order {

    void network_to_host(synapse::network::dns::packet::header& h);
    void network_to_host(synapse::network::dns::packet::query&  q);
    void network_to_host(synapse::network::dns::packet::answer& a);

}
}
}
}

void synapse::network::dns::byte_order::network_to_host(synapse::network::dns::packet::header& h)
{
    h.transaction_id   = ntohs(h.transaction_id);
    h.flag_int         = ntohs(h.flag_int);
    
    h.query_count      = ntohs(h.query_count);
    h.answer_count     = ntohs(h.answer_count);
    h.authorized_count = ntohs(h.authorized_count);
    h.additional_count = ntohs(h.additional_count);
}

void synapse::network::dns::byte_order::network_to_host(synapse::network::dns::packet::query&  q)
{
    q.query_type  = ntohs(q.query_type) ;
    q.query_class = ntohs(q.query_class);
}

void synapse::network::dns::byte_order::network_to_host(synapse::network::dns::packet::answer& a)
{
    a.answer_type   = ntohs(a.answer_type)  ;
    a.answer_class  = ntohs(a.answer_class) ;
    a.answer_ttl    = ntohl(a.answer_ttl)   ;
    a.answer_length = ntohs(a.answer_length);
}
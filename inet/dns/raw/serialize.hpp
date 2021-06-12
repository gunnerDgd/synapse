#include <synapse/inet/dns/packet/packet.hpp>
#include <synapse/inet/dns/format/name_format.hpp>
#include <synapse/inet/dns/format/byte_order.hpp>

namespace synapse {
namespace network {
namespace dns     {
namespace raw     {

    size_t serialize(synapse::network::dns::packet::query & s_query , char* s_memory);
    size_t serialize(synapse::network::dns::packet::answer& s_answer, char* a_memory);

    size_t serialize(char*                            s_memory,
                     std::string                      q_name  ,
                     synapse::network::dns::dns_type  q_type  ,
                     synapse::network::dns::dns_class q_class );

    size_t serialize(char*                            s_memory ,
                     std::string                      a_name   ,
                     synapse::network::dns::dns_type  a_type   ,
                     synapse::network::dns::dns_class a_class  ,
                     uint32_t                         a_ttl    ,
                     void*                            a_context,
                     uint16_t                         a_ctx_len);
}
}
}
}

size_t synapse::network::dns::raw::serialize(synapse::network::dns::packet::query& s_query, char* s_memory)
{
    return serialize(s_memory            ,
                     s_query.query_name  ,
                     s_query.query_type  ,
                     s_query.querry_class);
}

size_t synapse::network::dns::raw::serialize(synapse::network::dns::packet::answer& s_answer, char* s_memory)
{
    return serialize(s_memory,
                     s_answer.answer_name  ,
                     s_answer.answer_type  ,
                     s_answer.answer_class ,
                     s_answer.answer_ttl   ,
                     s_answer.answer_data  ,
                     s_answer.answer_length);
}

size_t synapse::network::dns::raw::serialize(char*                            s_memory,
                                             std::string                      q_name  ,
                                             synapse::network::dns::dns_type  q_type  ,
                                             synapse::network::dns::dns_class q_class )
{
    uint16_t q_name_length;
    char*    q_name = synapse::network::dns::name_format::host_to_network(q_name, q_name_length);
    memcpy  (s_memory, q_name, q_name_length); s_memory += sq_name_length;

    *(uint16_t*)s_memory = q_type ; s_memory += 2;
    *(uint16_t*)s_memory = q_class; s_memory += 2;

    return sq_name_length + 4;
}

size_t synapse::network::dns::raw::serialize(char*                            s_memory ,
                                             std::string                      a_name   ,
                                             synapse::network::dns::dns_type  a_type   ,
                                             synapse::network::dns::dns_class a_class  ,
                                             uint32_t                         a_ttl    ,
                                             void*                            a_context,
                                             uint16_t                         a_ctx_len)
{
    uint16_t a_name_length;
    char*    a_name       = synapse::network::dns::name_format::host_to_network(a_name, a_name_length);
    memcpy  (s_memory, sa_name, sa_name_length)  ; s_memory += sa_name_length;
    
    *(uint16_t*)s_memory = a_type   ; s_memory += 2;
    *(uint16_t*)s_memory = a_class  ; s_memory += 2;
    *(uint32_t*)s_memory = a_ttl    ; s_memory += 4;
    *(uint16_t*)s_memory = a_ctx_len; s_memory += 2;

    memcpy(s_memory, a_context, a_ctx_len);
    return a_name_length + a_ctx_len + 10;
}
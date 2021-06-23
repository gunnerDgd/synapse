#include <synapse/socket/socket.hpp>
#include <synapse/memory/vmem/vmem.hpp>

#include <synapse/inet/dns/packet/dns_packet.hpp>
#include <synapse/inet/dns/compress/decompress.hpp>

#include <synapse/inet/dns/raw/serialize.hpp>
#include <synapse/inet/dns/raw/deserialize.hpp>

#include <chrono>

namespace synapse {
namespace network {
namespace dns     {

    void          write_query (synapse::network::socket_base&        q_socket ,
                               synapse::network::dns::packet::header q_header ,
                               query_vector&                         q_context);

    void          write_answer(synapse::network::socket_base&        a_socket,
                               synapse::network::dns::packet::header a_header,  
                               query_vector &                        a_query ,
                               answer_vector&                        a_context);

    answer_vector read_answer (synapse::network::socket_base&        a_socket ,
                               answer_parser                         a_parser = nullptr);

    query_vector  read_query  (synapse::network::socket_base&        a_socket);

}
}
}

synapse::network::dns::answer_vector synapse::network::dns::read_answer (synapse::network::socket_base&       a_socket ,
                                                                         synapse::network::dns::answer_parser a_parser )
{    
    synapse::memory::vmem             a_buffer(4096);
    size_t q_flag     = a_socket.recv(a_buffer);
    
    if(q_flag == 0) 
        return synapse::network::dns::answer_vector(nullptr, 0);
    
    char* a_writer_pointer = (char*)a_buffer.memory_pointer(),
        * a_raw_pointer    = a_writer_pointer;

    synapse::network::dns::packet::header   a_header;
    synapse::network::dns::raw::deserialize(a_header, a_writer_pointer); 
    
    uint16_t a_count                      = a_header.answer_count + a_header.authorized_count + a_header.additional_count,
             a_offset                     = 0;

    synapse::network::dns::answer_vector a_res = { new synapse::network::dns::packet::answer[a_count]             , a_count};
    synapse::network::dns::query_vector  q_res = { new synapse::network::dns::packet::query [a_header.query_count], a_header.query_count};
    
    auto start = std::clock();

    if(a_header.query_count)
        for(uint16_t aq_it = 0 ; aq_it < a_header.query_count      ; aq_it++)
            synapse::network::dns::raw::deserialize(q_res.first[aq_it]   , a_writer_pointer, a_raw_pointer);
    
    if(a_header.answer_count)
        for(uint16_t aa_it = 0 ; aa_it < a_header.answer_count     ; aa_it++, a_offset++)
            synapse::network::dns::raw::deserialize(a_res.first[a_offset], a_writer_pointer, a_raw_pointer, a_parser);
        
    if(a_header.authorized_count)
        for(uint16_t au_it = 0 ; au_it < a_header.authorized_count ; au_it++, a_offset++)
            synapse::network::dns::raw::deserialize(a_res.first[a_offset], a_writer_pointer, a_raw_pointer, a_parser);

    if(a_header.additional_count)
        for(uint16_t ad_it = 0 ; ad_it < a_header.additional_count ; ad_it++, a_offset++)
            synapse::network::dns::raw::deserialize(a_res.first[a_offset], a_writer_pointer, a_raw_pointer, a_parser);

    auto end = std::clock();
    std::printf("%d\n", (end - start));

    return a_res;
}

void synapse::network::dns::write_query(synapse::network::socket_base&        q_socket ,
                                        synapse::network::dns::packet::header q_header ,
                                        query_vector&                         q_context)
{
    
    synapse::memory::vmem q_buffer(4096); // Buffer Space For Query.
    
    char                 *q_writer_pointer = (char*)q_buffer.memory_pointer(),
                         *q_raw_pointer    = q_writer_pointer;
    
    synapse::network::dns::raw::serialize    (q_header, q_writer_pointer);
    for(uint16_t q_it = 0 ; q_it < q_context.second ; q_it++)
        synapse::network::dns::raw::serialize(q_context.first[q_it], q_writer_pointer); // Query Serialization.

    // [QUERY] Sends Query.
    size_t q_flag     = q_socket.send((uint8_t*)q_raw_pointer, (size_t)(q_writer_pointer - q_raw_pointer));
}

void synapse::network::dns::write_answer(synapse::network::socket_base&        a_socket ,
                                         synapse::network::dns::packet::header a_header ,
                                         synapse::network::dns::query_vector & a_query  ,
                                         synapse::network::dns::answer_vector& a_context)
{
    synapse::memory::vmem a_buffer(4096);
    char*                 a_write_pointer = (char*)a_buffer.memory_pointer();

    synapse::network::dns::raw::serialize    (a_header, a_write_pointer);
    for(uint16_t q_it = 0 ; q_it < a_query.second   ; q_it++)
        synapse::network::dns::raw::serialize(a_query  .first[q_it], a_write_pointer);

    for(uint16_t a_it = 0 ; a_it < a_context.second ; a_it++)
        synapse::network::dns::raw::serialize(a_context.first[a_it], a_write_pointer);

    a_socket.send(a_buffer);
}
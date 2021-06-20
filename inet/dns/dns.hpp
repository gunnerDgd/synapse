#include <synapse/socket/socket.hpp>
#include <synapse/memory/vmem/vmem.hpp>

#include <synapse/inet/dns/packet/dns_packet.hpp>
#include <synapse/inet/dns/compress/decompress.hpp>

#include <synapse/inet/dns/raw/serialize.hpp>
#include <synapse/inet/dns/raw/deserialize.hpp>

namespace synapse {
namespace network {
namespace dns     {

    using query_vector  = std::vector<synapse::network::dns::packet::query> ;
    using answer_vector = std::vector<synapse::network::dns::packet::answer>; 

    answer_vector write_query (synapse::network::socket_base&        q_socket ,
                               synapse::network::dns::packet::header q_header ,
                               query_vector&                         q_context,
                               answer_parser                         q_parser = nullptr);

    void          write_answer(synapse::network::socket_base&        a_socket,
                               synapse::network::dns::packet::header a_header,
                               query_vector &                        a_query ,
                               answer_vector&                        a_context);

}
}
}

synapse::network::dns::answer_vector synapse::network::dns::write_query(synapse::network::socket_base&        q_socket ,
                                                                        synapse::network::dns::packet::header q_header ,
                                                                        query_vector&                         q_context,
                                                                        answer_parser                         q_parser )
{
    synapse::network::dns::answer_vector q_res         ;
    synapse::memory::vmem                q_buffer(4096); // Buffer Space For Query.
    
    char                                *q_writer_pointer = (char*)q_buffer.memory_pointer(),
                                        *q_raw_pointer    = q_writer_pointer;
    
    q_writer_pointer     += synapse::network::dns::raw::serialize(q_header, q_writer_pointer);
    for(auto& q_it : q_context)
        q_writer_pointer += synapse::network::dns::raw::serialize(q_it, q_writer_pointer); // Query Serialization.

    size_t q_flag    = q_socket.send((uint8_t*)q_raw_pointer, (uint64_t)(q_writer_pointer - q_raw_pointer));
    if    (q_flag == 0) return q_res;
    
    synapse::network::dns::packet::header   a_header;
    q_flag                  = q_socket.recv(q_buffer);

    q_writer_pointer = q_raw_pointer; // Initialize R/W Pointer.
    synapse::network::dns::raw::deserialize(a_header, q_writer_pointer);

    synapse::network::dns::query_vector     aq_res;
    for(uint16_t aq_it = 0 ; aq_it < a_header.query_count ; aq_it++) {
        synapse::network::dns::packet::query    aq;
        synapse::network::dns::raw::deserialize(aq, q_writer_pointer, q_raw_pointer);
        
        aq_res.push_back(aq);
    }

    for(uint16_t aa_it = 0 ; aa_it < a_header.answer_count ; aa_it++) {
        synapse::network::dns::packet::answer   aa;
        synapse::network::dns::raw::deserialize(aa, q_writer_pointer, q_raw_pointer, q_parser);

        q_res .push_back(aa);
    }

    return q_res;
}

void synapse::network::dns::write_answer(synapse::network::socket_base&        a_socket ,
                                         synapse::network::dns::packet::header a_header ,
                                         synapse::network::dns::query_vector & a_query  ,
                                         synapse::network::dns::answer_vector& a_context)
{
    synapse::memory::vmem a_buffer(4096);
    char*                 a_write_pointer = (char*)a_buffer.memory_pointer();

    synapse::network::dns::raw::serialize(a_header, a_write_pointer);
    a_write_pointer += 12;

    for(auto& aq_it : a_query)
        a_write_pointer += synapse::network::dns::raw::serialize(aq_it, a_write_pointer);

    for(auto& ac_it : a_context)
        a_write_pointer += synapse::network::dns::raw::serialize(ac_it, a_write_pointer);

    a_socket.send(a_buffer);
}
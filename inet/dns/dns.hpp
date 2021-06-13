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
                               query_vector&                         q_context);

    void          write_answer(synapse::network::socket_base&         a_socket ,
                               synapse::network::dns::packet::header a_header,
                               query_vector &                        a_query ,
                               answer_vector&                        a_context);

}
}
}

synapse::network::dns::answer_vector synapse::network::dns::write_query(synapse::network::socket_base&        q_socket ,
                                                                        synapse::network::dns::packet::header q_header ,
                                                                        query_vector&                         q_context)
{
    synapse::network::dns::answer_vector         q_res         ;
    synapse::memory::vmem                        q_buffer(4096);
    uint8_t*                                     q_writer_pointer = q_buffer.memory_pointer();
    
    synapse::network::dns::raw::serialize(q_header, q_writer_pointer);  
    
    q_writer_pointer += 12;
    for(auto& q_it : q_context)
        writer_size += synapse::network::dns::raw::serialize(q_it, q_writer_pointer);

    // Prepare to Send Query Packet (Serialization Progress)

    size_t q_flag    = q_socket.send(q_buffer.memory_pointer(), (size_t)(q_writer_pointer - q_buffer.memory_pointer()));
    q_writer_pointer = q_buffer.memory_pointer();

    if(q_flag == 0) 
        return q_res;
    
    synapse::network::dns::packet::header   a_header;
    synapse::network::dns::raw::deserialize(a_header, q_writer_pointer, q_buffer.memory_pointer());
    synapse::network::dns::query_vector     aq_res;

    for(uint16_t aq_it = 0 ; aq_it < a_header.query_count ; aq_it++) {
        synapse::network::dns::packet::query    aq;
        synapse::network::dns::raw::deserialize(aq, q_writer_pointer, q_buffer.memory_pointer())
        
        aq_res.push_back(aq);
    }

    for(uint16_t aa_it = 0 ; aa_it < a_header.answer_count ; aa_it++) {
        synapse::network::dns::packet::answer   aa;
        synapse::network::dns::raw::deserialize(aa, q_writer_pointer, q_buffer.memory_pointer());

        q_res .push_back(aa);
    }

    return q_res;
}

void synapse::network::dns::writer::write_answer(ynapse::network::socket_base&         a_socket ,
                                                 synapse::network::dns::packet::header a_header ,
                                                 query_vector &                        a_query  ,
                                                 answer_vector&                        a_context)
{
    synapse::memory::vmem a_buffer(4096);
    uint8_t*              a_write_pointer = a_buffer.memory_pointer();

    synapse::network::dns::raw::serialize(a_header, a_write_pointer);
    a_write_pointer += 12;

    for(auto& aq_it : a_query)
        a_write_pointer += synapse::network::dns::raw::serialize(aq_it, a_write_pointer);

    for(auto& ac_it : a_context)
        a_write_pointer += synapse::network::dns::raw::serialize(ac_it, a_write_pointer);

    q_socket.send(a_buffer.memory_pointer(), (size_t)(a_write_pointer - a_buffer.memory_pointer()));
}
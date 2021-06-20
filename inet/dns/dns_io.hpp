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

    void          write_query (synapse::network::socket_base&        q_socket ,
                               synapse::network::dns::packet::header q_header ,
                               query_vector&                         q_context);

    void          write_answer(synapse::network::socket_base&        a_socket,
                               synapse::network::dns::packet::header a_header,  
                               query_vector &                        a_query ,
                               answer_vector&                        a_context);

    answer_vector read_answer (synapse::network::socket_base&        q_socket ,
                               answer_parser                         q_parser = nullptr);

}
}
}

synapse::network::dns::answer_vector synapse::network::dns::read_answer (synapse::network::socket_base&       a_socket ,
                                                                         synapse::network::dns::answer_parser a_parser )
{
    synapse::network::dns::answer_vector a_res         ;
    synapse::memory::vmem                a_buffer(4096);
    
    size_t q_flag     = a_socket.recv(a_buffer);
    if    (q_flag == 0) return a_res;
    
    char* a_writer_pointer = (char*)a_buffer.memory_pointer(),
        * a_raw_pointer    = a_writer_pointer;
           

    synapse::network::dns::packet::header   a_header;
    
    synapse::network::dns::raw::deserialize(a_header, a_writer_pointer);
    synapse::network::dns::query_vector     aq_res;

    std::cerr << "[REQUEST] ## RCODE  : " << (int)a_header.flag.reply   << std::endl;
    std::cerr << "[REQUEST] ## QCOUNT : " << (int)a_header.query_count  << std::endl;
    std::cerr << "[REQUEST] ## ACOUNT : " << (int)a_header.answer_count << std::endl;

    for(uint16_t aq_it = 0 ; aq_it < a_header.query_count ; aq_it++) {
        synapse::network::dns::packet::query    aq;
        synapse::network::dns::raw::deserialize(aq, a_writer_pointer, a_raw_pointer);
        
        aq_res.push_back(aq);
    }

    for(uint16_t aa_it = 0 ; aa_it < a_header.answer_count ; aa_it++) {
        synapse::network::dns::packet::answer   aa;
        synapse::network::dns::raw::deserialize(aa, a_writer_pointer, a_raw_pointer, a_parser);

        a_res .push_back(aa);
    }

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
    for(auto& q_it : q_context)
        synapse::network::dns::raw::serialize(q_it    , q_writer_pointer); // Query Serialization.

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

    synapse::network::dns::raw::serialize(a_header, a_write_pointer);
    for(auto& aq_it : a_query)
        synapse::network::dns::raw::serialize(aq_it, a_write_pointer);

    for(auto& ac_it : a_context)
        synapse::network::dns::raw::serialize(ac_it, a_write_pointer);

    a_socket.send(a_buffer);
}
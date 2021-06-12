#include <synapse/socket/socket.hpp>
#include <synapse/memory/vmem/vmem.hpp>
#include <synapse/inet/dns/packet/packet.hpp>

namespace synapse {
namespace network {
namespace dns     {
    
    class writer : private synapse::memory::vmem
    {
    public:
        using query_vector  = std::vector<synapse::network::dns::packet::query> ;
        using answer_vector = std::vector<synapse::network::dns::packet::answer>;   

    public:
        writer()
            : synapse::memory::vmem(4096),
              writer_wrptr         (this->memory_pointer()) { }

        void write_query (synapse::network::dns::packet::header q_header,
                          query_vector&                         q_context);

        void write_answer(synapse::network::dns::packet::header a_header,
                          answer_vector&                        a_context);

    private:
        uint8_t* writer_wrptr;
    };

}
}
}

void writer::synapse::network::dns::writer::write_query(synapse::network::dns::packet::header q_header,
                                                        query_vector&                         q_context)
{
    
}
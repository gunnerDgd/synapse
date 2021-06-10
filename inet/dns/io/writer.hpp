#include <synapse/socket/socket.hpp>
#include <synapse/inet/dns/packet/packet.hpp>
#include <synapse/memory/vmem/vmem.hpp>

namespace synapse {
namespace network {
namespace dns     {
    
    class writer : private synapse::memory::vmem
    {
    public:
        writer()
            : synapse::memory::vmem(4096),
              writer_wrptr         (this->memory_pointer()) { }
        
        void write_header(synapse::network::dns::dns_opcode      h_op,
                          synapse::network::dns::packet_type     h_type);
        void write_header(synapse::network::dns::packet::header& h) ;

        void write_query (std::string q_name,
                          synapse::network::dns::);
        void write_answer();

    private:
        uint8_t* writer_wrptr;
    };

}
}
}

void synapse::network::dns::writer::write_header(synapse::network::dns::dns_opcode  h_op,
                                                 synapse::network::dns::packet_type h_type)
{
    
}

void synapse::network::dns::writer::write_header(synapse::network::dns::packet::header& h)
{
    writer_wrptr = 
}
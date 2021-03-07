#include <synapse/inet/dns/packet/packet.hpp>
#include <synapse/inet/dns/format/compress.hpp>
#include <synapse/socket/socket.hpp>

namespace dns
{
    class writer
    {
    public:
        void write_header(uint16_t   tID, 
                          flag_field flag,
                          uint16_t   qcount,
                          uint16_t   acount    = 0,
                          uint16_t   authcount = 0,
                          uint16_t   addcount  = 0);
        
        void write_query (std::string q_name, uint16_t q_type, uint16_t q_class);
        void write_answer(std::string a_name, 
                          uint16_t    a_type, 
                          uint16_t    a_class, 
                          uint32_t    a_ttl  ,
                          uint16_t    a_data_length,
                          std::string a_data);

        void write_to    (socket::socket_base& s);

    private:
        uint8_t write_block[4096] = {0x00, },
               *write_pointer     = write_block;
    };
}

void dns::writer::write_header(uint16_t   tID, 
                               flag_field flag,
                               uint16_t   qcount,
                               uint16_t   acount   ,
                               uint16_t   authcount,
                               uint16_t   addcount )
{
    write_pointer   = write_block;
    uint16_t* h_ptr = (uint16_t*)write_pointer;

    *h_ptr++ = htons(tID);
    *h_ptr++ = htons((uint16_t)flag);
    
    *h_ptr++ = htons(qcount)   ;
    *h_ptr++ = htons(acount)   ;
    *h_ptr++ = htons(authcount);
    *h_ptr++ = htons(addcount) ;
}
        
void dns::writer::write_query (std::string q_name, uint16_t q_type, uint16_t q_class)
{
    name::hton      (q_name, (char*)write_pointer); 
    write_pointer  += strlen((char*)write_pointer) + 1;
    
    uint16_t* q_ptr = (uint16_t*)write_pointer;

    *q_ptr++ = htons(q_type) ;
    *q_ptr++ = htons(q_class);
}
void dns::writer::write_answer(std::string a_name, 
                               uint16_t    a_type, 
                               uint16_t    a_class, 
                               uint32_t    a_ttl  ,
                               uint16_t    a_data_length,
                               std::string a_data)
{
    name::hton     (a_name, (char*)write_pointer);
    write_pointer += strlen((char*)write_pointer) + 1;

    uint16_t* a_ptr = (uint16_t*)write_pointer;
    *a_ptr++        = htons(a_type) ;
    *a_ptr++        = htons(a_class);

    *(uint32_t*)a_ptr = 
}
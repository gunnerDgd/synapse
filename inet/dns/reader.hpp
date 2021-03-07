#include <synapse/inet/dns/packet/packet.hpp>
#include <synapse/inet/dns/format/compress.hpp>
#include <synapse/socket/socket.hpp>

#include <unordered_map>

namespace dns
{
    class reader
    {
    public:
        using q_vec_t  = std::vector<query> ;
        using a_vec_t  = std::vector<answer>;
        
        using cb_vec_t = std::unordered_map<uint16_t, parse_callback>;
        using cb_t     = std::string(*)(char* b, char* r);

    public:
        bool read_from    (socket::socket_base& s);
        void read_callback(uint16_t type, parse_callback cb);
        
    private:
        void parse_query (uint8_t*& b, uint8_t* r);
        void parse_answer(uint8_t*& b, uint8_t* r);

    public:
        header&  read_header() { return dns_header  ;  }
        q_vec_t& read_query () { return query_vector;  }
        a_vec_t& read_answer() { return answer_vector; }

    private:
        cb_vec_t parse_vec;
        
        q_vec_t  query_vector ;
        a_vec_t  answer_vector;
        header   dns_header   ;
    };
}

bool dns::reader::read_from(socket::socket_base& s)
{
    uint8_t   rd_block[4096],
             *rd_pointer = rd_block;

    if(s.read(rd_block, 4096) == 0) return false;
    memcpy(&dns_header, rd_pointer, 12); rd_pointer += 12;
    
    dns_header.transaction_id   = ntohs(dns_header.transaction_id);
    dns_header.flag_int         = ntohs(dns_header.flag_int);

    dns_header.query_count      = ntohs(dns_header.query_count);
    dns_header.answer_count     = ntohs(dns_header.answer_count);
    dns_header.authorized_count = ntohs(dns_header.authorized_count);
    dns_header.additional_count = ntohs(dns_header.additional_count);

    parse_query (rd_pointer, rd_block);
    parse_answer(rd_pointer, rd_block);
}

void dns::reader::parse_query (uint8_t*& b, uint8_t* r)
{
    for(uint16_t q_it = 0 ; q_it < dns_header.query_count ; q_it++)
    {
        query q;
        
        q.query_name  = name::ntoh(b, r)    ; b += strlen((char*)b) + 1;
        q.query_type  = htons(*(uint16_t*)b); b += 2;
        q.query_class = htons(*(uint16_t*)b); b += 2;

        query_vector.push_back(q);
    }
}
void dns::reader::parse_answer(uint8_t*& b, uint8_t* r)
{
    for(uint16_t a_it = 0 ; a_it < dns_header.answer_count; a_it++)
    {
        answer a;

        a.answer_name   = name::ntoh(b, r)    ; b += strlen((char*)b) + 1;
        a.answer_type   = htons(*(uint16_t*)b); b += 2;
        a.answer_class  = htons(*(uint16_t*)b); b += 2;

        a.answer_ttl    = htonl(*(uint32_t*)b); b += 4;
        a.answer_length = htons(*(uint16_t*)b); b += 2;

        auto r_cb_it    = parse_vec.find(a.answer_type);
        if  (r_cb_it   == parse_vec.end())
            a.answer_data = "";
        else
            a.answer_data = *r_cb_it(b, r);

        answer_vector.push_back(a);
        b       += a.answer_length;
    }
}
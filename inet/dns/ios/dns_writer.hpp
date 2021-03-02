#include <synapse/inet/dns/dns_packet.hpp>

namespace dns
{
    class writer
    {
    public:
        bool write_to(socket::socket_base& s) { s << wr_buf; }

    public:
        void write_header(header::flag_field& f,
                          uint64_t            qc,
                          uint64_t            ac    = 0,
                          uint64_t            authc = 0,
                          uint64_t            addc  = 0);
        
        void write_answer(std::string n, answer::answer_context c);
        void write_query (std::string n, query::query_context   q);
    

    private:
        uint8_t  wr_buf[4096] = { 0x00, };
        uint64_t  wr_ptr      = 0;
    };
}

void dns::writer::write_header(header::flag_field& f    ,
                               uint64_t            qc   ,
                               uint64_t            ac   ,
                               uint64_t            authc,
                               uint64_t            addc)
{
    wr_ptr         = 0;
    raw::header* h = (raw::header*)wr_buf;
    
    memcpy(&h->dns_flags_field, &f, sizeof(f));

    h->question      = qc   ;
    h->answer_rr     = ac   ;
    h->auth_rr       = authc;
    h->additional_rr = addc ; wr_ptr += 12;
}

void dns::writer::write_answer(std::string n, answer::answer_context c)
{
    char*  n_cvt = name::hton(n);
    byte_order::hton         (c);
    
    memcpy(wr_buf + wr_ptr, n_cvt, strlen(n_cvt)); wr_ptr += strlen(n_cvt) + 1;
    memcpy(wr_buf + wr_ptr, &c   , sizeof(c))    ; wr_ptr += sizeof(c);

    delete[] n_cvt;
}

void dns::writer::write_query (std::string n, query::query_context   q)
{
    char* n_cvt = name::hton(n);
    byte_order::hton        (q);

    memcpy(wr_buf + wr_ptr, n_cvt, strlen(n_cvt) + 1); wr_ptr += strlen(n_cvt) + 1;
    memcpy(wr_buf + wr_ptr, &q   , sizeof(q))        ; wr_ptr += sizeof(q);

    delete[] n_cvt;
}
#include <synapse/inet/dns/format/parsed.hpp>
#include <synapse/inet/dns/format/raw.hpp>
#include <synapse/socket/tcp.hpp>
#include <synapse/socket/udp.hpp>

namespace dns
{
    class reader
    {
    public:
        bool read_from(socket::socket_base& s);

    public:
        std::vector<parsed::query> & read_query ();
        std::vector<parsed::answer>& read_answer();

    private:
        uint8_t  rd_buf[4096];
        uint64_t rd_ptr   = 0;

        std::vector<parsed::query>  rd_qry;
        std::vector<parsed::answer> rd_ans;
    };
}

bool dns::reader::read_from(socket::socket_base& s)
{
    rd_ptr  = 0;
    s >> rd_buf;

    raw::header* rh = (raw::header*)rd_buf;
    
    
}
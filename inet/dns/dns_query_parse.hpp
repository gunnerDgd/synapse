#include <synapse/synapse.hpp>

#include <synapse/inet/dns/packet/dns_type.hpp>
#include <synapse/inet/dns/format/name_format.hpp>
#include <synapse/inet/dns/compress/decompress.hpp>

namespace synapse {
namespace network {
namespace dns     {
namespace parse   {

    void* parse_cname(uint16_t c_type, char*& c_data, char* c_raw);
    void* parse_ipv4 (uint16_t c_type, char*& c_data, char* c_raw);

    // parse_ipv4 can call parse_cname if c_type is CNAME type.

}
}
}
}

void* synapse::network::dns::parse::parse_cname(uint16_t c_type, char*& c_data, char* c_raw)
{
    char*  parse_res;
    synapse::network::dns::compress::decompress_name(c_data, c_raw, parse_res);

    return parse_res;
}

void* synapse::network::dns::parse::parse_ipv4(uint16_t c_type, char*& c_data, char* c_raw)
{
    if(c_type == (uint16_t)synapse::network::dns::dns_type::cname)
        return parse_cname(c_type, c_data, c_raw);
    else if (c_type == (uint16_t)synapse::network::dns::dns_type::a)
    {
        char*  parse_res = new char[4];
        memcpy(parse_res, c_data, 4);

        return parse_res;
    }
}
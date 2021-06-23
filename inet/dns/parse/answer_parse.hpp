#include <synapse/synapse.hpp>

#include <synapse/inet/dns/packet/dns_type.hpp>
#include <synapse/inet/dns/format/name_format.hpp>
#include <synapse/inet/dns/compress/decompress.hpp>

namespace synapse {
namespace network {
namespace dns     {
namespace parse   {

    // parse_ipv4 can call parse_cname if c_type is CNAME type.
    std::string parse_cname(uint16_t c_type, char* c_data, char* c_raw) { return synapse::network::dns::compress::decompress_name(c_data, c_raw); }
    std::string parse_ipv4 (uint16_t c_type, char* c_data, char* c_raw);

}
}
}
}

std::string synapse::network::dns::parse::parse_ipv4(uint16_t c_type, char* c_data, char* c_raw)
{
    if(c_type == (uint16_t)synapse::network::dns::dns_type::cname)
        return parse_cname(c_type, c_data, c_raw);

    if(c_type != (uint16_t)synapse::network::dns::dns_type::a)
        return std::string(""); // Wrong type.

    std::string parse_res = std::to_string((uint8_t)c_data[0]) + "."
                          + std::to_string((uint8_t)c_data[1]) + "."
                          + std::to_string((uint8_t)c_data[2]) + "."
                          + std::to_string((uint8_t)c_data[3]);

    return parse_res;
}
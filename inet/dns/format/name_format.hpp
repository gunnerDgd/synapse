#include <synapse/string/string_tools.hpp>

namespace synapse     {
namespace network     {
namespace dns         {
namespace name_format {

    std::string network_to_host(char*       net_name);
    char*       host_to_network(std::string hst_name, uint16_t& hst_len);
    void        host_to_network(std::string hst_name, char* hst_dst);

}
}
}
}

std::string synapse::network::dns::name_format::network_to_host(char* net_name) { return std::string(net_name + 1, (size_t)*net_name); }
char*       synapse::network::dns::name_format::host_to_network(std::string hst_name, uint16_t& hst_len)
{
    std::vector<std::string> hst_col   = synapse::string::split(hst_name, ".");
    hst_len                            = 0;
    
    char*                    hst_res   = new char[hst_name.length() + 2],
        *                    hst_wrptr = hst_res;

    for(auto& c_it : hst_col)
    {
        hst_len      += *hst_wrptr + 1;
        *hst_wrptr++  = c_it.length() ;

        memcpy(hst_wrptr, c_it.c_str(), c_it.length()); 
        hst_wrptr      += c_it.length() + 1;
    }

    hst_len     ++;
    return hst_res;
}

void        synapse::network::dns::name_format::host_to_network(std::string hst_name, char* hst_dst)
{
    std::vector<std::string> hst_col   = synapse::string::split(hst_name, ".");
    char*                    hst_wrptr = hst_dst;

    for(auto& c_it : hst_col)
    {
        *hst_wrptr++    = c_it.length();
        memcpy(hst_wrptr, c_it.c_str(), c_it.length()); hst_wrptr += c_it.length() + 1;
    }

    
}
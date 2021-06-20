#pragma once
#include <synapse/string/string_tools.hpp>

#include <cstdlib>
#include <cstring>

namespace synapse     {
namespace network     {
namespace dns         {
namespace name_format {

    std::string network_to_host        (char* net_name); // Convert All Name, Using synapse::string::split.
    std::string network_to_host_partial(char* net_name) { return std::string(net_name + 1, (size_t)*net_name); } // Convert One Column of the Name.

    char*       host_to_network(std::string hst_name, uint16_t& hst_len); // Allocate String to the Newly Allocated Buffer.
    uint16_t    host_to_network(std::string hst_name, char*&    hst_dst); // Copy String to the Dedicated Buffer.

}
}
}
}

/*

** DNS Name Field Format

hst_col --> Splitted Strings, splitted as "." token.

ex) www.google.com
hst_col[0] = www
hst_col[1] = google
hst_col[2] = com


+-------------------+------------+
|                   |            |
| Name Field Length | Name Field |
|                   |            |
+-------------------+------------+

*hst_wrptr --> Name Field Length. Size : 1 Byte
           --> *hst_wrptr++ for name field writing operation.
*/

std::string synapse::network::dns::name_format::network_to_host(char* net_name) 
{
    std::string ntoh_res;

    while(*net_name != NULL)
    {
        ntoh_res += std::string(net_name + 1, (size_t)(*net_name)) + ".";
        net_name += *net_name + 1;
    }

    return ntoh_res;
}

char*       synapse::network::dns::name_format::host_to_network(std::string hst_name, uint16_t& hst_len)
{
    std::vector<std::string> hst_col   = synapse::string::split(hst_name, ".");
    hst_len                            = hst_name.length() + 2;
    
    char*                    hst_res   = new char[hst_len],
        *                    hst_wrptr = hst_res;

    for(auto& c_it : hst_col)
    {
        *hst_wrptr++    = c_it.length() ;

        memcpy(hst_wrptr, c_it.c_str (), c_it.length()); 
        hst_wrptr      += c_it.length();
    }
    return hst_res;
}

uint16_t synapse::network::dns::name_format::host_to_network(std::string hst_name, char*& hst_dst)
{
    std::vector<std::string> hst_col   = synapse::string::split(hst_name, ".");
    uint16_t                 hst_len   = 0;

    for(auto& c_it : hst_col)
    {
        *hst_dst++         = c_it.length();
        std::memcpy(hst_dst, c_it.c_str (), c_it.length());

        hst_dst += c_it.length();
        hst_len += c_it.length() + 1; // 1 : Padding Byte.
    }   hst_dst ++;

    return hst_len;
}
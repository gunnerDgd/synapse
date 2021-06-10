#pragma once
#include <iostream>
#include <synapse/inet/dns/format/name_format.hpp>

/*
How to decompress DNS Message.

1) If DNS name string's first character bit starts as 0xc0,
the message is compressed.

2) Compressed message field is...

+-------------------+--------------------+--------------------+
| Compression Flags | Compression Offset | Compression Offset |
+-------------------+--------------------+--------------------+
0					2					 8					  16

!! NOTICE !!
Compression field goes on at the last of the message field.
Like, www.google.0xc0 0x0c.

3) Type of the compressed message.

<1> All of the name field is compressed.
ex) 0xc0 0x0c --> 12 Byte offset, all of the message is compressed.

<2> Part of the name field is compressed.
ex) www.google.0xc0 0x0c


4) ntoh (Network Format to Host Format)

DNS Network Format

+--------------+------------------+
| Field Length | Field Name Label |
+------------- +------------------+
0              8                  N

DNS Host Format

+--------------+------------------+
| Field Length | Field Name Label |
+------------- +------------------+
0              8                  N

*/

namespace synapse  {
namespace network  {
namespace dns      {
namespace compress {

    struct compression_flag
    {
        uint16_t compression_offset : 14;
        uint16_t compression_flag   : 2;
    };
    // Struct comp_field
    // --> Saves DNS Compression Field, Which contains compression flag and offset.

    // Check this flag is compressed.
    bool        check_compression(char& ch)       { return (ch & 0xc0); }
    
    // Find the Compression Offset.
    size_t      find_offset(char* find_raw) { return (compression_flag*)find_raw->compression_offset; }
    char*       find_name  (char* cp_name, char* cp_raw);

    std::string decompress_name(char*& cp_name, char* cp_raw);
}
}
}
}

char*  synapse::network::dns::compress::find_name(char* cp_name, char* cp_raw)
{
    while(check(*cp_name))
        cp_name = cp_raw + offset(cp_name);
        
    return cp_name;
}

std::string synapse::network::dns::compress::decompress_name(char*& cp_name, char* cp_raw)
{
    std::string dc_res;
    while     (*cp_name != NULL)
    {
        if(check(*cp_name)) 
        {
            char* dc_name  = find_name(cp_name, cp_raw);
            dc_res        += synapse::network::dns::name_format::network_to_host(dc_name);

            cp_name += *dc_name + 2;
        }
        else
        {
            dc_res     .append(cp_name + 1, *cp_name);
            cp_name += *cp_name + 2;
        }
    }

    cp_name    ++;
    return dc_res;
}
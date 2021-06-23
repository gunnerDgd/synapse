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
        uint16_t compression_flag   : 2;
        uint16_t compression_offset : 14;
    };
    // Struct comp_field
    // --> Saves DNS Compression Field, Which contains compression flag and offset.

    // Check this flag is compressed.
    bool        check_compressed(char* find_raw) { return *find_raw & 0xc0; }
    
    uint16_t    find_offset     (char* find_raw);
    char*       find_name       (char* cp_name, char* cp_raw);

    std::string decompress_name(char*& cp_name, char* cp_raw);
    void        decompress_name(char*& cp_name, char* cp_raw, char* cp_dest);
}
}
}
}

/*

1) find_name(char*, char*)

[1] Parameters

cp_name : Pointer of the Compressed Name.
cp_raw  : Start Point of the raw packet, which includes header, queries, and answers.

[2] Working

<1> Check the First Byte to Find if the name is compressed.
<2> If the name is compressed, follow the compression_offset flag until function finds original name string.\


2) decompress_name

[1] Parameters

cp_name : Pointer of the Compressed Name.
cp_raw  : Start Point of the raw packet, which includes header, queries, and answers.

[2] Working

<1> Check String Snippet if the snippet is compressed.
--> If Snippet is Compressed, Find the Name Offset using "find_name" function.
--> If Snipped isn't Compressed, Just add offset to cp_name variable.

*/


char*  synapse::network::dns::compress::find_name(char* cp_name, char* cp_raw)
{
    while(check_compressed(cp_name))
        cp_name = cp_raw + find_offset(cp_name);

    return cp_name;
}

uint16_t synapse::network::dns::compress::find_offset(char* find_raw) 
{
    if(!check_compressed(find_raw))
        return 0;
    else
        return (uint16_t)((find_raw[0] ^ 0xc0) << 8) | find_raw[1];
}

std::string synapse::network::dns::compress::decompress_name(char*& cp_name, char* cp_raw)
{
    std::string dc_res;
    while     (*cp_name != '\0')
    {
        switch(check_compressed(cp_name))
        {
        case true:
        {
            char* dc_ptr = find_name(cp_name, cp_raw);
            dc_res      += synapse::network::dns::compress::decompress_name(dc_ptr, cp_raw);
            
            cp_name += 2;
            return     dc_res;
        }

        case false:
            dc_res  += synapse::network::dns::name_format::network_to_host_partial(cp_name) + ".";
            cp_name += *cp_name + 1; // 1 for Padding Byte. 
            
            break;
        }
    }       cp_name ++; // For Skipping NULL Byte.
    
    if(!dc_res.empty()) dc_res.erase(dc_res.length() - 1, 1);
    return              dc_res;
}

void        synapse::network::dns::compress::decompress_name(char*& cp_name, char* cp_raw, char* cp_dest)
{
    std::string dc_res = decompress_name(cp_name, cp_raw);
    memcpy     (cp_dest, dc_res.c_str(), dc_res.length());
}
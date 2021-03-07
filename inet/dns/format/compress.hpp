#pragma once
#include <iostream>
#include <synapse/string/string_tools.hpp>

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

namespace dns
{
    struct comp_field
    {
        uint16_t compression_offset : 14;
        uint16_t compression_flag   : 2;
    };
    // Struct comp_field
    // --> Saves DNS Compression Field, Which contains compression flag and offset.

    bool        is_compressed(char& c) { return (c & (0xc0 << 8)); }
    class name
    {
    public:
        static std::string ntoh      (char*  msg, char* raw);
        static void        hton      (std::string  name, char* n_res);
    private:
        static std::string decompress(char* msg, char* raw);
    };
    
    
}

std::string dns::name::ntoh      (char* msg, char* raw)
{
    std::string ntoh_res;
    while(*msg)
    {
        std::string dc = decompress(msg, raw);
        
        msg           += (is_compressed(*msg)) ? 2 : dc.length();
        ntoh_res      += dc;
    }

    ntoh_res.pop_back();
    return   ntoh_res;
}

void        dns::name::hton      (std::string name, char* n_res)
{
    auto   n_vec = string::split(name, "."); 
    size_t r_size = 0;
    
    for(auto& v : n_vec)
    {
        n_res[r_size++] = v.length();
        memcpy           (n_res + r_size, v.c_str(), v.length());

        r_size         += v.length(); 
    }

    return n_res;
}

std::string dns::name::decompress(char* msg, char* raw)
{
    switch(is_compressed(*msg))
    {
    case true:
    {
        comp_field* cf = reinterpret_cast<comp_field*>(msg);
        return      decompress (raw + cf->compression_offset, raw);
    }

    case false:
        return      std::string(msg + 1, *msg) + ".";
    }
}
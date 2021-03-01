#include <iostream>

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


    bool        is_compressed(char& c) { return (c & 0xc0); }
    
    std::string ntoh_name (char* name, uint8_t* raw);
    char*       hton_name (std::string& name);

    std::string decompress(comp_field* comp, uint8_t* raw);
    std::string decompress(char* name);
}

std::string dns::decompress(char* name)
{
    uint8_t n_len = *name++;
    return std::string(name, n_len) + ".";
}
std::string dns::decompress(comp_field* cp_field, uint8_t* cp_raw)
{
    std::string decomp_result;
    if(!cp_field.compression_flag) return decomp_result; // Nothing to decompress.
    
    
}

std::string dns::ntoh_name(char* name, uint8_t* raw)
{
    std::string ntoh_res;
    while(*name)
    {
        if(is_compressed(*name)) {
            ntoh_res += decompress((comp_field*)name, raw); // End of the Name Field.
            break;
        }

        auto d_res = decompress(name); // ntoh Result.
        name      += d_res.length();
        ntoh_res  += d_res;
    }
}
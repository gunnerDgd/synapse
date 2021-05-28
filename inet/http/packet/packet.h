#include <synapse/c_container/list/list.h>
#include <string.h>
#include <stdlib.h>

typedef struct http_prefix
{
    char* prefix_method ,
        * prefix_path   ,
        * prefix_version;
} http_prefix;

typedef struct http_packet
{
    http_prefix * packet_prefix ;
    list_context* packet_header ;

    const char  * packet_context;
}   http_packet;

size_t http_parse_prefix (http_packet* http_pkt, char* http_raw_pkt);
size_t http_parse_header (http_packet* http_pkt, char* http_raw_pkt);


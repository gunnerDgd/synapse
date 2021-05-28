#include "packet.h"

size_t http_parse_prefix (http_packet* http_pkt, char* http_raw_pkt)
{
    char* prefix_bound = strstr(http_raw_pkt, "\r\n"),
        * prefix_token;
    if   (prefix_bound == NULL) 
        return 0;

    memset   (prefix_bound, 0x00, 2);
    http_pkt->packet_prefix = http_raw_pkt;
    
    prefix_token = strtok(http_raw_pkt, " "); 

    return (size_t)(prefix_bound - http_raw_pkt);
}
size_t http_parse_header (http_packet* http_pkt, char* http_raw_pkt);
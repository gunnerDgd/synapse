#include <synapse/inet/http/packet/packet.h>

http_request_prefix* http_request_parse_prefix   (const char** http_raw);
http_request_packet* http_request_parse_packet   (const char*  http_raw);
void                 http_destruct_request_packet(http_request_packet* del_pkt);
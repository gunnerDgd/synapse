#include <synapse/inet/http/packet/packet.h>

http_response_prefix* http_response_parse_prefix   (const char** http_raw);
http_response_packet* http_response_parse_packet   (const char*  http_raw);
void                  http_destruct_response_packet(http_response_packet* del_pkt);
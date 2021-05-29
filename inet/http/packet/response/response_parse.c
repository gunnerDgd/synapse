#include "response_parse.h"

http_response_prefix* http_response_parse_prefix(const char** http_raw)
{
    char                * http_end_prefix = strstr(http_raw, http_row_token);
    memset               (http_end_prefix, 0x00, 2);

    http_response_prefix* http_parse_result = malloc(sizeof(http_response_prefix));
    if  (!http_end_prefix) 
        return NULL;

    http_parse_result->response_version = strtok(*http_raw, " ");
    http_parse_result->response_code    = atoi  (strtok(NULL, " ") + 1);
    http_parse_result->response_message = strtok(NULL, " ") + 1;

    *http_raw = http_end_prefix + http_row_token_size;
    return      http_parse_result;
}

http_response_packet* http_response_parse_packet(const char*  http_raw)
{
    http_response_packet* http_parse_result = malloc(sizeof(http_response_packet));
    http_parse_result->packet_raw           = http_raw;

    http_parse_result->packet_prefix  = http_response_parse_prefix(&http_raw)
    http_parse_result->packet_context = strstr(http_raw, http_context_token) + http_context_token_size;

    http_parse_result->packet_header  = list_create();
    http_header                       * http_parsed_header;
    
    while(http_parsed_header = http_response_parse_header(&http_raw))
        list_insert(http_parse_result->packet_header, -1, http_parsed_header, 8);
    
    return http_parse_result;
}
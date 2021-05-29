#include "request_parse.h"

http_request_prefix* http_request_parse_prefix   (const char** http_raw)
{
    http_request_prefix* http_parse_result = malloc(sizeof(http_request_prefix));
    char               * http_end_prefix   = strstr(*http_raw, http_row_token);

    if(!http_end_prefix) {
        free(http_parse_result);
        return             NULL;
    }

    http_parse_result->request_method      = strtok(*http_raw, " ");
    http_parse_result->request_path        = strtok(NULL, " ");
    http_parse_result->request_version     = strtok(NULL, " ");

    return http_parse_result;
}

http_request_packet* http_request_parse_packet   (const char*  http_raw)
{
    http_request_packet* http_parse_result = malloc(sizeof(http_request_packet));
    http_parse_result->packet_raw          = http_raw;

    http_parse_result->packet_context      = strstr(http_raw, http_context_token) + http_context_token_size;
    http_parse_result->packet_prefix       = http_request_parse_prefix(&http_raw);

    http_header        * http_parsed_header;
    while(http_parsed_header = http_parse_header(&http_raw))
        list_insert(http_parse_result->packet_header, -1, http_parsed_header, 8);
}

void                 http_destruct_request_packet(http_request_packet* del_pkt);
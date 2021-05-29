#pragma once

#include <synapse/c_container/list/list.h>
#include <string.h>
#include <stdlib.h>

#define http_row_token_size     2
#define http_context_token_size 4

#define http_row_token     "\r\n"
#define http_context_token "\r\n\r\n"

typedef struct http_request_prefix
{
    char* request_method ,
        * request_path   ,
        * request_version;
}              http_request_prefix;

typedef struct http_response_prefix
{
    char  * response_version,
          * response_message;
    
    uint8_t response_code;
}              http_response_prefix;

typedef struct http_header
{
    char* header_prefix,
        * header_context;
}              http_header;

typedef struct http_request_packet
{
    http_request_prefix * packet_prefix ;
    list_context        * packet_header ;

    const char          * packet_context,
                        * packet_raw    ;
}              http_request_packet;

typedef struct http_response_packet
{
    http_response_prefix * packet_prefix ;
    list_context         * packet_header ;

    const char           * packet_context,
                         * packet_raw    ;
}              http_response_packet;

http_header* http_parse_header(const char** http_raw)
{
    http_header* http_parse_result    = malloc(sizeof(http_header));
    char       * http_header_end      = strstr(*http_raw, http_row_token);

    if(!http_header_end) // End of the HTTP Header Part.
        goto parse_error;

    http_parse_result->header_prefix  = strtok(http_raw, ":");
    http_parse_result->header_context = strtok(NULL, ":")    ;

    *http_raw = http_header_end + http_row_token_size;
    return      http_parse_result;

parse_error:
    if(http_parse_result->header_prefix)  free(http_parse_result->header_prefix) ;
    if(http_parse_result->header_context) free(http_parse_result->header_context);

    free(http_parse_result);
    return             NULL;
}
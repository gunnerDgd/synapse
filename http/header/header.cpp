#include "header.hpp"

http::packet::packet(const char* _ctx, size_t _len, packet_type _type) : p_type(_type)
{
    p_context            = strstr(_ctx, "\r\n\r\n") + strlen("\r\n\r\n");
    size_t      h_length = (p_context == nullptr) ? _len : ((size_t)(p_context - _ctx) - 4),
                h_ptr    = 0;

    context_size         = _len - h_length - 4;
    h_context            = new char[context_size];
    memcpy                (h_context, p_context, context_size);

    format::string_list p_column = format::string::split(std::string(_ctx, h_length),
                                                         "\r\n"
                                                        );

    switch(p_type)
    {
        case packet_type::res:
            p_response = new response(p_column[0]);
            break;

        case packet_type::req:
            p_request  = new request(p_column[0]);
            break;
    }

    for(size_t i = 1; i < p_column.size() ; i++)
    {
        header i_header(p_column[i]);
        p_header.insert(std::make_pair(i_header.h_prefix, std::move(i_header)));
    }
}

http::response::request    (std::string& r_ctx)
{
    format::string_list ctx_field = format::string::split(r_ctx, " ");
    
    r_method                       = std::move(ctx_field[0]);
    r_url                          = std::move(ctx_field[1]);
    r_version                      = std::move(ctx_field[2]);
}

http::response::response   (std::string& r_ctx)
{
    format::string_list ctx_field = format::string::split(r_ctx, " ");
    
    r_version                     = std::move(ctx_field[0]);
    r_status                      = std::move(ctx_field[1]);
    r_message                     = std::move(ctx_field[2]);
}

http::header::header       (std::string& _ctx)
{
    format::string::trim(_ctx, " ");
    format::string_list ctx_field = format::string::split(_ctx, ":");

    h_prefix                      = std::move(ctx_field[0]);
    h_context                     = std::move(ctx_field[1]);
}
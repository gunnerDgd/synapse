#include <iostream>
#include <cstring>

#include <synapse/format/string.hpp>

namespace http
{
    class request
    {
        public:
            request    (std::string& r_ctx);
            request    (request&&    r_move)
            {
                r_method  = std::move(r_move.r_method);
                r_url     = std::move(r_move.r_url);
                r_version = std::move(r_move.r_version);
            }

            std::string r_method,
                        r_url,
                        r_version;
    };

    class response
    {
        public:
            response   (std::string& r_ctx);
            response   (response&&   r_move)
            {
                r_version = std::move(r_move.r_version);
                r_status  = std::move(r_move.r_status);
                r_message = std::move(r_move.r_message);
            }

            std::string r_version,
                        r_status,
                        r_message;
    };

    class header
    {
        public:
            header     (std::string& _ctx);
         
            std::string h_prefix,
                        h_context;
    };

    class packet
    {
        using      header_map = std::map<std::string, header>;
        enum       packet_type { req, res };

        public:
            packet (const char* _ctx, size_t _len, packet_type _type);
            ~packet() 
            { 
                if(p_type == packet_type::req) delete p_request;
                else                           delete p_response;

                delete[]                              p_context;
            }

        public:
            union
            {
                request    *p_request;
                response   *p_response;
            };  packet_type p_type;

            header_map      p_header;
            
            char*           p_context;
            size_t          context_size;
    };
}
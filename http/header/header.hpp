#include <iostream>
#include <cstring>

#include <synapse/format/string.hpp>

namespace http
{
    class request
    {
        public:
            request    (std::string& r_ctx);
            request    (std::string _meth, std::string _url, std::string _ver)
                       : r_method (std::move(_meth)),
                         r_url    (std::move(_url)),
                         r_version(_ver) {}

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
            response   (std::string _ver, std::string _stat, std::string _msg)
                       : r_version(std::move(_ver)),
                         r_status (std::move(_stat)),
                         r_message(std::move(_msg)) {}

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
            header     (std::string _pref, std::string _ctx)
                       : h_prefix (std::move(_pref)),
                         h_context(std::move(_ctx)) {}
         
            std::string h_prefix,
                        h_context;
    };

    class packet
    {
        using      header_map = std::map<std::string, header>;
        enum       packet_type { req, res };

        public:
            packet (const char* _ctx, size_t _len, packet_type _type);
            packet (void*  _pref, header_map&  _header, const char* _context, packet_type _type)
                   : p_header (std::move(_header)),
                     p_context(_context),
                     p_type   (_type),
                     p_request(_pref) {}

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
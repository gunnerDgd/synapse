#include "packet.hpp"

namespace http
{
    class request : public packet
    {
        public:
            request    (const char* r_raw);
            request    (std::string _meth, std::string _url, std::string _ver);
            request    (request&&    r_move);

            std::string r_method,
                        r_url,
                        r_version;
    };

    request::request(const char* r_raw) : p_context(strstr(r_raw, "\r\n\r\n")), 
                                          p_raw(r_raw)
    {
        memset (p_context, 0x00, 4); p_context += 4;
        format::string_list r_column = format::string::split(std::string(r_raw), "\r\n");
        format::string_list r_req    = format::string::split(r_column[0],        " ");

        r_method  = std::move(r_req[0]);
        r_url     = std::move(r_req[1]);
        r_version = std::move(r_req[2]);

        for(int i = 1 ; i < r_column.size() ; i++) add_header(h_column(r_column[i]));
    }

    request::request    (std::string _meth, std::string _url, std::string _ver)
                        : r_method (std::move(_meth)),
                          r_url    (std::move),
                          r_version(_ver) {}

    request::request    (request&&    r_move)
    {
        r_method  = std::move(r_move.r_method);
        r_url     = std::move(r_move.r_url);
        r_version = std::move(r_move.r_version);
    }
}
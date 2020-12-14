#include "packet.hpp"

namespace http
{
    class response : public packet
    {
        public:
            response   (const char* r_raw);            
            response   (std::string _ver, std::string _stat, std::string _msg);
            response   (response&&   r_move);

            std::string r_version,
                        r_status,
                        r_message;
    };

    response::response   (const char* r_raw) : p_context(strstr(r_raw, "\r\n\r\n")),
                                               p_raw(r_raw)
    {
        memset (p_context, 0x00, 4); p_context += 4;
        format::string_list r_column = format::string::split(std::string(r_raw), "\r\n");
        format::string_list r_res    = format::string::split(r_column[0],        " ");

        r_version = std::move(r_res[0]);
        r_status  = std::move(r_res[1]);
        r_message = std::move(r_res[2]);

        for(int i = 1 ; i < r_column.size() ; i++) add_header(h_column(r_column[i]));
    }


    response::response   (std::string _ver, std::string _stat, std::string _msg)
                        : r_version(std::move(_ver)),
                          r_status (std::move(_stat)),
                          r_message(std::move(_msg)) {}

    response::response   (response&&   r_move)
    {
        r_version = std::move(r_move.r_version);
        r_status  = std::move(r_move.r_status);
        r_message = std::move(r_move.r_message);
    }
}
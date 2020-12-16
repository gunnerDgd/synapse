#include "packet.hpp"

namespace http
{
    class response : public packet
    {
        public:
            response   (char* r_raw);            
            response   (std::string _ver, std::string _stat, std::string _msg);
            response   (response&&   r_move);

            friend network::tcp& operator << (network::tcp& _s, response& _r)
            {
                _s                            << (_r.r_netmsg);
                
				for(auto& h : _r.p_header) _s << (h.h_netmsg);
				_s							  << "\r\n";

				return _s;
            }

            std::string r_version,
                        r_status,
                        r_message;

            std::string r_netmsg;
    };

    response::response (char* r_raw) : packet(r_raw)
    {
        if(p_context != nullptr)     { memset (p_context, 0x00, 4); p_context += 4; }

        format::string_list r_column = format::string::split(std::string(r_raw), "\r\n");
        format::string_list r_res    = format::string::split(r_column[0],        " ");

        r_version = std::move(r_res[0]);
        r_status  = std::move(r_res[1]);
        r_message = std::move(r_res[2]);

        r_netmsg  = r_version + " " + r_status + " " + r_message + "\r\n";

        for(int i = 1 ; i < r_column.size() ; i++) write_header(std::move(header(r_column[i])));
    }

    response::response   (std::string _ver, std::string _stat, std::string _msg)
                        : packet   (),
                          r_version(std::move(_ver)),
                          r_status (std::move(_stat)),
                          r_message(std::move(_msg)),
                          r_netmsg (r_version + " " + r_status + " " + r_message + "\r\n") {}

    response::response   (response&&   r_move)
    {
        r_version = std::move(r_move.r_version);
        r_status  = std::move(r_move.r_status);
        r_message = std::move(r_move.r_message);

        r_netmsg  = r_version + " " + r_status + " " + r_message + "\r\n";
    }
}
#include "http_packet.hpp"

http::request::request  (const char* _raw_msg)
{
    std::istringstream               req_str(_raw_msg);
    req_str >> r_method >> r_path >> r_version;
}

http::response::response(const char* _raw_msg)
{
    std::istringstream                req_str;
    req_str >> r_version >> r_code >> r_msg;
}

http::message::message(const char* _msg)
{
    std::list<std::string> msg = format::string::split(std::string(_msg), "\r\n");

    

    delete[] _msg;
}
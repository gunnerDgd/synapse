#include "string.hpp"

void                  format::string::trim (std::string& target, std::string delim)
{
    size_t seek_cur = 0;
    for   ( ; (seek_cur = delim.find(seek_next, delim)) != std::string::npos ; )
        target.erase(seek_cur, delim.length());
}

std::list<std::string> format::string::split(std::string& target, std::string delim)
{
    size_t                 seek_cur = 0, seek_next = 0;
    std::list<std::string> seek_res;

    for   ( ; (seek_cur = target.find(seek_next, delim)) != std::string::npos ; )
    {
        seek_res                    .push_back(target.substr(seek_cur,
                                                             seek_cur - seek_next));
        seek_next = seek_cur + delim.length();
    }

    seek_res.push_back(target.substr(seek_next, target.length() - seek_next));
    return   seek_res;
}

std::set<size_t>      format::string::find (std::string& target, std::string delim)
{
    size_t           seek_cur = 0, seek_next = 0;
    std::set<size_t> seek_res;

    for   ( ; (seek_cur = target.find(seek_next, delim)) != std::string::npos ; )
    {
        seek_res                    .insert(seek_cur);
        seek_next = seek_cur + delim.length();
    }

    return   seek_res;
}
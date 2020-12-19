#include "string.hpp"

void                format::string::trim (std::string& target, std::string delim)
{
    size_t seek_cur = 0;
    for   ( ; (seek_cur = target.find(delim, seek_cur)) != std::string::npos ; )
        target.erase(seek_cur, delim.length());
}

format::string_list format::string::split(std::string  target, std::string delim)
{
    size_t              seek_cur = 0, seek_next = 0;
    format::string_list seek_res;

    for   ( ; (seek_cur = target.find(delim, seek_next)) != std::string::npos ; )
    {
        seek_res                    .push_back(target.substr(seek_next,
                                                             seek_cur - seek_next));
        seek_next = seek_cur + delim.length();
    }

    seek_res.push_back(target.substr(seek_next, target.length() - seek_next));
    return   seek_res;
}

format::offset_list format::string::find (std::string& target, std::string delim)
{
    size_t              seek_cur = 0, seek_next = 0;
    format::offset_list seek_res;

    for   ( ; (seek_cur = target.find(delim, seek_next)) != std::string::npos ; )
    {
        seek_res                    .insert(seek_cur);
        seek_next = seek_cur + delim.length();
    }

    return   seek_res;
}
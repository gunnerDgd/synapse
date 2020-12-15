#pragma once
#include "header.hpp"

namespace http
{
    using header_vec = std::vector<header>;
    class packet
    {
        public:
            packet ()             {}
            packet (char* _raw) : p_context(std::strstr(_raw, "\r\n\r\n")), 
                                  p_raw    (_raw)                         {}
            ~packet()                                                     { if(p_raw != nullptr) delete[] p_raw; }

            void      write_header(header h_add)                          { p_header.push_back(h_add); }

        public:
            header_vec  p_header;
            char*       p_context = nullptr;
            char*       p_raw     = nullptr;
    };                             
}
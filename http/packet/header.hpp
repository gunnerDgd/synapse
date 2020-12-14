#pragma once
#include <iostream>
#include <string>

#include <synapse/format/string.hpp>

namespace http
{
    class header
    {
        public:
            header()   {}
            header     (std::string& _ctx)
            {
                format::string::trim(_ctx, " ");
                format::string_list ctx_field = format::string::split(_ctx, ":");

                h_prefix                      = std::move(ctx_field[0]);
                h_context                     = std::move(ctx_field[1]);
            }

            header     (std::string _pref, std::string _ctx)
                       : h_prefix (std::move(_pref)),
                         h_context(std::move(_ctx)) {}

            header     (header&& h_move) : h_prefix (std::move(h_move.h_prefix)),
                                           h_context(std::move(h_move.h_context)) {}
         
            std::string h_prefix,
                        h_context;
    };


}
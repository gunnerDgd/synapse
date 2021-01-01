#pragma once
#include <iostream>
#include <string>
#include <cstring>

#include <synapse/format/string.hpp>
#include <synapse/socket/tcp/tcp.hpp>

namespace http
{
    class header
    {
        public:
            header()   {}
            header(std::string& _ctx) : h_netmsg(_ctx)
            {
                format::string::trim(_ctx, " ");
                format::string_view_list ctx_field = format::string::split_view(_ctx, ":");

                h_prefix                      = std::move(ctx_field[0]);
                h_context                     = std::move(ctx_field[1]);
            }

            header     (std::string _pref, std::string _ctx)
                       : h_prefix (_pref),
                         h_context(_ctx),
                         h_netmsg (h_prefix + ": " + h_context + "\r\n") {}

            header     (header&& h_move) : h_prefix (std::move(h_move.h_prefix)),
                                           h_context(std::move(h_move.h_context)),
                                           h_netmsg (std::move(h_move.h_netmsg)) { }

			header	   (const header& h_copy)
			{
				h_prefix  = h_copy.h_prefix;
				h_context = h_copy.h_context;
				h_netmsg  = h_copy.h_netmsg;
			}
         
            std::string h_prefix,
                        h_context;
            std::string h_netmsg;
    };


}
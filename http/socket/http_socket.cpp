#include "http_socket.hpp"

http::socket::socket(network::tcp& _sock) : _sock(&_sock)
{
    http_socket->on_recv = std::bind(&socket::message_handler, this, 
                                     std::placeholders::_1,
                                     std::placeholders::_2,
                                     std::placeholders::_3);
}

void http::socket::add_handler    (std::string _header,
                                   handler_t   _handler)   { http_handler.insert(std::make_pair(_header, _handler)); }
            
void http::socket::delete_handler (std::string _header) { http_handler.erase(_header); }

void http::socket::message_handler(network::tcp& _sock,
                                   uint8_t*      _msg ,
                                   size_t        _recv)
{
    http::message  msg(_msg);
    for(auto& _m : msg.msg_header)
    {
        auto _h = http_handler .find(_m.first);
        if  (_h == http_handler.end()) continue;

        _h.second(*this, _m.second.h_context);
    }
}
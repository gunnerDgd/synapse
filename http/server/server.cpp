#include "server.hpp"

http::http_server::http_server   (uint16_t _port, std::string _path) 
                                : network::tcp_server("0.0.0.0", _port)
{
    on_client = std::bind(&http::http_server::process_request, this, std::placeholders::_1);
}

void http::http_server::process_request(network::tcp* _cl)
{
    if (!on_http_client) return;

    char*  _cl_msg = new char[HTTP_BUFSIZE];
    memset(_cl_msg, 0x00,     HTTP_BUFSIZE);

    _cl          ->recv((uint8_t*)_cl_msg, HTTP_BUFSIZE);

    request*		 _cl_req = new request(_cl_msg);
    on_http_client  (_cl, _cl_req);
}
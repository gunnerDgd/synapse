#include "server.hpp"

http::http_server::http_server   (uint16_t _port, std::string _path) 
                                : network::tcp_server("0.0.0.0", _port)
{
    on_client = std::bind(&http::http_server::process_request, this, std::placeholders::_1);
}

void http::http_server::process_request(network::tcp* _cl)
{
    if (!on_http_client) return;

    memory::memory_block<uint8_t[HTTP_BUFSIZE]>* _cl_msg = http_mpool.acquire();
    memset										(_cl_msg->get(), 0x00, HTTP_BUFSIZE);

    _cl          ->recv(_cl_msg->get(), HTTP_BUFSIZE);

    request*		 _cl_req = new request((char*)_cl_msg->get());
    on_http_client  (_cl, _cl_req);
}
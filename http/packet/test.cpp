#include <synapse/http/packet/header.hpp>
#include <synapse/http/server/server.hpp>

int main()
{
    http::http_server t_serv(6000, "/");
    if(!t_serv.start_server()) return 0;

    const char* test_http = "\r\n<html>Hello World</html>";

    t_serv.on_http_client = [&](network::tcp* _s, http::request _r) 
    { 
        http::response _res("HTTP/1.1", "200", "OK");
        *_s << _res;
        _s->send((uint8_t*)test_http, strlen(test_http));

    };

    while(true) {usleep(4000);}
}
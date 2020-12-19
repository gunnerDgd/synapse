#include <synapse/http/packet/header.hpp>
#include <synapse/http/server/server.hpp>

int main()
{
    http::http_server t_serv(6500, "/");
    if(!t_serv.start_server()) return 0;

    const char* test_http = "<html>Hello World</html>";

    t_serv.on_http_client = [&](network::tcp* _s, http::request* _r)
    { 
        http::response _res("HTTP/1.1", "200", "OK");
		_res.write_header  ("Content-Type",   "text/html");
        _res.write_header  ("Content-Length", std::to_string(strlen(test_http)));

        *_s << _res;
        _s->send((uint8_t*)test_http, strlen(test_http));

        delete _r;
    };

    while(true) {usleep(4000*1000);}
}
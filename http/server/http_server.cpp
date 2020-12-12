#include "http_server.hpp"

http::server::server()
{
    http_server.on_connection =  [this](network::tcp cl)
                                ->void { this->on_connection(cl); };
}

void http::server::start() { http_server.start(); }
void http::server::end  () { http_server.end(); }
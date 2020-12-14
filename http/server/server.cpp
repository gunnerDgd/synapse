#include "server.hpp"

http::http_server::http_server   (uint16_t _port, std::string _path)
{
    connection_handler = [&, this](network::tcp* _cl)
                         {
                            char*  _cl_msg = new char[4096];
                            memset(_cl_msg, 0x00, 4096);

                            _cl         ->recv(_cl_msg, 4096);
                            if(on_client) on_client(_cl, packet(_cl_msg));
                         };
}
#include "server.hpp"

http::http_server::http_server   (uint16_t _port, std::string _path)
{
    connection_handler = [&, this](network::tcp* _cl)
                         {
                            char*  _cl_msg = new char[HTTP_BUFSIZE];
                            memset(_cl_msg, 0x00, HTTP_BUFSIZE);

                            _cl         ->recv     (_cl_msg, HTTP_BUFSIZE);
                            if(on_client) on_client(_cl, packet(_cl_msg));
                            else          delete[]  _cl_msg;
                         };
}
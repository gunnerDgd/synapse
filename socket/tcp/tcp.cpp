#include "tcp.hpp"

network::tcp::tcp(const char* _ip, unsigned short _port) 
            : network::socket_base(_ip, _port),
              socket_fd           (::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) {}

network::tcp::tcp(network::socket_type _sock, sockaddr_in& _addr)
            : network::socket(_sock, _addr) {}

bool   network::tcp::connect   ()
{
    bool res =  (::connect(socket_fd,
                           reinterpret_cast<sockaddr*>&socket_address,
                           sizeof(sockaddr_in)) == 0) ? true : false;

    if(!res && error_handler) error_handler(error::connect_fail);
    return                    res;
}

void   network::tcp::disconnect() { close(socket_fd); }

size_t network::tcp::send(uint8_t* s_ctx, size_t s_size
{
    if(stream_mode == stream::mode::sync) read_lock.acquire();
    size_t send_res = ::send(socket_fd, s_ctx, s_size, 0);

    if(send_res > 0 && error_handler) error_handler(error::send_fail);
    if(on_send)                     { on_send      (*this, send_res); return 0; }
    else                              return send_res;
}

size_t network::tcp::recv(uint8_t* r_ctx, size_t r_size)
{
    if(stream_mode == stream::mode::sync) write_lock.acquire();
    size_t recv_res = ::send(socket_fd, r_ctx, r_size, 0);

    if(recv_res > 0 && error_handler) error_handler(error::recv_fail);
    if(on_send)                     { on_recv      (*this, r_ctx, send_res); return 0; }
    else                              return recv_res;
}
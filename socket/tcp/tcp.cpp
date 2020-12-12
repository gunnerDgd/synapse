#include "tcp.hpp"

network::tcp::tcp  (std::string& tcp_ip, uint16_t tcp_port) { set_address(tcp_ip, tcp_port); }

network::tcp::~tcp()                                              { disconnect(); }

void   network::tcp::set_address(std::string& _ip, uint16_t _port)
{
        disconnect();
        
        sock_address.sin_addr.s_addr = inet_addr(_ip.c_str());
        sock_address.sin_port        = htons    (_port);
        sock_address.sin_family      = AF_INET;

        sock_ctx   = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

bool   network::tcp::connect()
{
        bool res = (::connect(sock_ctx, (struct sockaddr*)&sock_address, sizeof(struct sockaddr_in)) == 0) ? true : false;
        on_connected        (*this);
}

void   network::tcp::disconnect()
{
        close          (sock_ctx);
        on_disconnected(*this);
}

size_t network::tcp::send(uint8_t* s_ctx, size_t s_size)
{
        size_t send_size = ::send(sock_ctx, s_ctx, s_size, 0);
        on_send                  (*this, s_size);

        return send_size;
}

size_t network::tcp::recv(uint8_t* r_ctx, size_t r_size)
{
        size_t recv_size = ::recv(sock_ctx, r_ctx, r_size, 0);
        on_recv                  (*this, r_ctx, r_size);

        return recv_size;
}
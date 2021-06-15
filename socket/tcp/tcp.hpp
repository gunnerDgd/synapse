#pragma once
#include <synapse/socket/socket.hpp>

namespace synapse {
namespace network {

    class tcp : public synapse::network::socket_base
    {
    public:
        tcp (synapse::network::ip::v4 t_addr);
        tcp (socket_type               t_fd   ,
             synapse::network::ip::v4 t_addr);

        bool connect   ();
        bool disconnect();

    public:
        size_t send(uint8_t* s_ctx, size_t s_size) override;
        size_t recv(uint8_t* r_ctx, size_t r_size) override;

    protected:
        
    };
}
}

synapse::network::tcp::tcp (synapse::network::ip::v4& t_addr)
    : socket_base(t_addr)
{
    socket_descriptor     = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    if(socket_descriptor <= 0)
        stream_state_flag = synapse::io::io_state::internal_error;
}

synapse::network::tcp::tcp (socket_type               t_fd  ,
                            synapse::network::ip::v4 t_addr)
    : socket_base(t_fd, t_addr) { }

bool synapse::network::tcp::connect   ()
{
    int conn_res = ::connect(socket_descriptor             ,
                            &socket_address.address_context,
                             sizeof(sockaddr_in));

    return (conn_res < 0) ? false : true;
}

bool synapse::network::tcp::disconnect()
{
#ifdef UNIX_MODE
	close      (socket_descriptor);
#else
	closesocket(socket_descriptor);
#endif
}

size_t synapse::network::tcp::send(uint8_t* s_ctx, size_t s_size)
{
    if(io_state_flag  != synapse::io::io_state::normal)
        return 0;
    
    ssize_t snd_res = ::send(socket_descriptor,
                             s_ctx,
                             s_size,
                             0);
    
    if(snd_res < 0) {
        io_state_flag = synapse::io::io_state::io_error;
        return 0;
    } else {
        return snd_res ;
    }
}

size_t synapse::network::tcp::recv(uint8_t* r_ctx, size_t r_size)
{
    if(io_state_flag  != synapse::io::io_state::normal)
        return 0;
    
    ssize_t rcv_res = ::recv(socket_descriptor,
                             r_ctx,
                             r_size,
                             0);
    
    if(rcv_res < 0) {
        io_state_flag = synapse::io::io_state::io_error;
        return 0;
    } else {
        return rcv_res;
    }
}
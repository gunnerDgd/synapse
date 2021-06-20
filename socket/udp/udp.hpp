#pragma once
#include <synapse/socket/socket.hpp>

namespace synapse {
namespace network {

	class udp : public synapse::network::socket_base
	{
	public:
		udp (synapse::network::ip::v4 	     u_addr);
        udp (synapse::network::socket_handle u_sock,
			 synapse::network::ip::v4	     u_addr);
		
	public:
		bool  connect   (); // Connect socket to the kernel.
		void  disconnect() { close(socket_descriptor); }
		
	public:
        size_t send(uint8_t* s_ctx, size_t s_size) override;
        size_t recv(uint8_t* r_ctx, size_t r_size) override;
	};
	
}
}

synapse::network::udp::udp(synapse::network::ip::v4 u_addr)
	: socket_base(u_addr)
{
	socket_descriptor = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(socket_descriptor <= 0)
		io_state_flag = synapse::io::io_state::internal_error;
}

synapse::network::udp::udp(synapse::network::socket_handle u_sock,
				  		   synapse::network::ip::v4	       u_addr)
	: socket_base(u_sock, u_addr) { }


bool   synapse::network::udp::connect()
{
    if(socket_descriptor <= 0) return false;
    int conn_res = ::connect(socket_descriptor,
                            (sockaddr*)&socket_address.address_context(),
                             sizeof(sockaddr_in));

    if(conn_res < 0)
        io_state_flag = synapse::io::io_state::internal_error;

    return (conn_res == 0) ? true : false;
}

size_t synapse::network::udp::send(uint8_t* s_ctx, size_t s_size)
{
    if(io_state_flag  != synapse::io::io_state::normal)
        return 0;
    
    ssize_t snd_res = ::send(socket_descriptor,
                             s_ctx,
                             s_size,
                             0);
    
    if(snd_res < 0) 
    {
        io_state_flag = synapse::io::io_state::io_error;
        return 0;
    } else
        return snd_res ;
}

size_t synapse::network::udp::recv(uint8_t* r_ctx, size_t r_size)
{
    if(io_state_flag  != synapse::io::io_state::normal)
        return 0;
    
    ssize_t rcv_res = ::recv(socket_descriptor,
                             r_ctx,
                             r_size,
                             0);
    
    if(rcv_res < 0) 
    {
        io_state_flag = synapse::io::io_state::io_error;
        return 0;
    } else
        return rcv_res;
}
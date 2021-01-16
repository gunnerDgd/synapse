#include "tcp_server.hpp"

network::tcp_server::tcp_server(const char* _ip, unsigned short _port)
{
#ifdef WIN32_MODE
	WSAStartup(MAKEWORD(2, 2), &server_ws2data);
#endif

	server_address.sin_addr.s_addr = inet_addr(_ip);
	server_address.sin_port		   = htons(_port);
	server_address.sin_family	   = AF_INET;

	server_socket				   = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

void network::tcp_server::end  ()
{
	server_running = false;

#ifdef UNIX_MODE
	close(server_socket);
#else
	closesocket(server_socket);
#endif

	if (server_thread != nullptr) server_thread->join();
}

bool network::tcp_server::start()
{
	bool start_res = (::bind(server_socket,
					  reinterpret_cast<sockaddr*>(&server_address),
					  sizeof(sockaddr_in)) != -1) ? true : false;

	start_res 	   = (::listen(server_socket, 20) != -1) ? true : false;
	server_running = true;
	
	server_thread = new std::thread([&]()
	{
		network::socket_type cl_socket;

		sockaddr_in          cl_address;
		int                  cl_size = sizeof(sockaddr_in);

		while (server_running)
		{
			cl_socket = ::accept(server_socket,
								 reinterpret_cast<sockaddr*>(&cl_address),
								(uint32_t*)&cl_size);

			network::tcp   *cl = new network::tcp(cl_socket, cl_address);
			wait 	   		   = cl;
		}
	});

	return true;
}
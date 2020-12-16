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

void network::tcp_server::end_server()
{
	server_running = false;
#ifdef UNIX_MODE
	close(server_socket);
#else
	closesocket(server_socket);
#endif

	if (server_thread != nullptr) server_thread->join();
}

bool network::tcp_server::start_server()
{
	bool start_res = (::bind(server_socket,
		reinterpret_cast<sockaddr*>(&server_address),
		sizeof(sockaddr_in)) != -1) ? true : false;

	if (!start_res) { if (on_error) on_error(this, error::bind_error);   return false; }

	start_res = (::listen(server_socket, 20) != -1) ? true : false;
	if (!start_res) { if (on_error) on_error(this, error::listen_error); return false; }

	server_running = true;
	server_thread = new std::thread([&]()
	{
		network::socket_type cl_socket;

		sockaddr_in          cl_address;
		int                  cl_size = sizeof(sockaddr_in);

		while (server_running == true)
		{
			cl_socket = ::accept(server_socket,
								 reinterpret_cast<sockaddr*>(&cl_address),
								 (int*)&cl_size);

			if (cl_socket == -1) { on_error(this, error::server_ended); break; }
			network::tcp   *cl = new network::tcp(cl_socket, cl_address);

			on_client(cl);
		}
	});

	if (on_server)  on_server(this, error::server_started);
	return true;
}
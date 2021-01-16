#include <synapse/http/packet/http_message.hpp>
#include <synapse/socket/tcp/server/tcp_server.hpp>

#include <functional>

namespace http
{
	class server : private network::tcp_server
	{
	public:
		server (uint16_t port);
		~server();
		
		void start_server();
		void end_server  ();
	
	public:
		using on_client = std::function<void(network::tcp*, http::request_message)>;
		on_client		  client_handler;
	
	private:
		std::thread		 *sv_thread;
		bool			  sv_running = false;
		
		void			  sv_process();
	};
}

http::server::server(uint16_t port)
	: network::tcp_server("0.0.0.0", port) { start_server(); }

http::server::~server()
{
	end_server();
	delete sv_thread;
}

void http::server::start_server() 
{ 
	if(sv_running) return;
	start();
	
	sv_running = true;
	sv_thread  = new std::thread(&server::sv_process, this);
}

void http::server::end_server  () 
{ 
	if(!sv_running) return;
	end();
	
	sv_running = false; 
	sv_thread  ->join();
	
	delete		 sv_thread;
}

void http::server::sv_process()
{
	while(sv_running)
	{
		network::tcp* cl 	  = wait.wait();
		char*		  cl_data = new char[4096];
		
		if(!client_handler){
			delete cl_data;
			continue;
		} else client_handler(cl, http::request_message(cl_data, 4096));
	}
}
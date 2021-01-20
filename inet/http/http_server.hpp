#include <synapse/inet/http/packet/http_message.hpp>
#include <synapse/socket/tcp/server/tcp_server.hpp>

#include <functional>

namespace http
{
	class server : private network::tcp_server
	{
	public:
		server (uint16_t port);
		~server();
		
		void 			  sv_start();
		void 			  sv_end  ();
	
	public:
		using on_client = std::function<void(network::tcp*, http::request_message)>;
		on_client		  sv_handler;
	
	private:
		std::thread		 *sv_thread;
		bool			  sv_running = false;
		
		void			  sv_process();
	};
}

http::server::server(uint16_t port)
	: network::tcp_server("0.0.0.0", port) { sv_start(); }

http::server::~server()
{
	sv_end();
	delete sv_thread;
}

void http::server::sv_start() 
{ 
	if(sv_running) return;
	start();
	
	sv_running = true;
	sv_thread  = new std::thread(&server::sv_process, this);
}

void http::server::sv_end  () 
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
		} else sv_handler(cl, http::request_message(cl_data, 4096));
	}
}
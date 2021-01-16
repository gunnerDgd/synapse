#include <synapse/socket/tcp/tcp.hpp>
#include <synapse/sync/signal.hpp>
#include <thread>

namespace network
{
    class tcp_server
    {
        public:
            tcp_server (const char* _ip, unsigned short _port);
            ~tcp_server() { end(); }

        public:
            bool 							   start();
            void 							   end  ();
			synchronous::signal<network::tcp*> wait	  ;
		
        private:
            sockaddr_in          			   server_address;
            network::socket_type 			   server_socket ;
#ifdef WIN32_MODE
			WSADATA				 			   server_ws2data;
#endif
		private:
            std::thread         			  *server_thread = nullptr;
            bool                			   server_running;
    };
}
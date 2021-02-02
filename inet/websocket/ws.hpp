#include <synapse/socket/tcp/tcp.hpp>
#include <synapse/inet/http/packet/http_message.hpp>
#include <synapse/inet/dns/dns_socket.hpp>

namespace websocket
{
	class ws
	{
	public:
		ws(char* ws_url);
		ws(char* ws_ip, uint16_t ws_port);
		
	public:
		bool connect   ();
		void disconnect();
		
	public:
		enum   type { text, binary }
		
		size_t send(uint8_t* ctx, size_t len, type s_type);
		size_t recv(uint8_t* ctx, size_t len);
		
	public:
		
		
	private:
		network::tcp* ws_tcp;
	};
}
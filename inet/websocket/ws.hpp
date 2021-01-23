#include <synapse/socket/tcp/tcp.hpp>

namespace websocket
{
	class ws
	{
	public:
		ws();
		
	public:
		bool connect   ();
		void disconnect();
		
	public:
		
	public:
		
		
	private:
		network::tcp* ws_tcp;
	};
}
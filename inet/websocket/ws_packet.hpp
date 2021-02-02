#include <iostream>
#include <variant>

namespace  websocket
{
	enum   ws_opcode
	{
		conn_continue = 0x00,
		text	      = 0x01,
		binary		  = 0x02,
		conn_close    = 0x03,
		ping		  = 0x09,
		pong		  = 0x0A
	};
	
	struct ws_header
	{
		uint16_t fin     : 1;
		
		uint16_t rsv1    : 1;
		uint16_t rsv2    : 1;
		uint16_t rsv3    : 1;
		
		uint16_t opcode  : 4;
		uint16_t mask    : 1;
		uint16_t payload : 7;
	};
	
	struct ws_data
	{
		uint8_t  ws_lenType;
		uint64_t ws_len;
		uint8_t* ws_data;
	};
	
	struct ws_frame
	{
		ws_header header;
		ws_data   data;
	};
}
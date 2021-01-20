#include <synapse/socket/socket.hpp>
#include <synapse/string/string_tools.hpp>

#include <variant>

namespace http
{
	class message
	{
	public:
		message(char* raw, size_t len);
		message() {}
		
		void write_prefix(std::string v, unsigned short c, std::string m);
		void write_prefix(std::string m, std::string u   , std::string v);
		
		void write_header(std::string prefix, std::string context);
		
	public:
		friend network::socket_base& operator >> (network::socket_base& s,
											      message&				m)
		{
			char*  msg_buffer = new char[4096];
			size_t msg_recv   = s.recv((uint8_t*)msg_buffer, 4096);

			if(msg_recv)
				m = message(msg_buffer, msg_recv);

			return s;
		}
		
		friend network::socket_base& operator << (network::socket_base& s,
												  message& 			    m)
		{
			if(!m.msg_prefix.index())
				s.send((uint8_t*)std::get<std::string_view>(m.msg_prefix).data(), 
					   			 std::get<std::string_view>(m.msg_prefix).length());
			else
				s.send((uint8_t*)std::get<std::string>(m.msg_prefix).c_str(), 
					   			 std::get<std::string>(m.msg_prefix).length());
			
			for(auto& m_it : m.msg_header) {
				if(!m_it.index())
					s.send((uint8_t*)std::get<std::string_view>(m_it).data(), 
						   			 std::get<std::string_view>(m_it).length());
				else
					s.send((uint8_t*)std::get<std::string>(m_it).c_str(), 
						   			 std::get<std::string>(m_it).length());
			}
			
			s << "\r\n";
			s.send(m.msg_context, m.msg_ctx_size); return s;
		}
		
	public:
		using msg_col_type 	    = std::variant<std::string_view, std::string>;
		
		msg_col_type 			  msg_prefix;
		std::vector<msg_col_type> msg_header;
		
		uint8_t*				  msg_context;
		size_t					  msg_ctx_size;
	};
}

http::message::message(char* raw, size_t len)
{
	msg_context     = (uint8_t*)strstr(raw, "\r\n\r\n");
	
	if(msg_context == nullptr) 
		return; // Malformed;
	else
		msg_context += 0x04;
	
	size_t mborder = (size_t)msg_context - (size_t)raw;
	msg_ctx_size   = (!msg_context) ? 0
							  	    : len - mborder;

	std::string_view			  mv	  (raw, mborder);
	std::vector<std::string_view> mv_col = string::split(mv, "\r\n");
	
	if(mv_col.size() == 0) 
		return; // Malformed;
	
	msg_prefix	   = mv_col[0];
	for(size_t it  = 1 ; it < mv_col.size() ; it++)
		msg_header.push_back(mv_col[it]);
}

void http::message::write_prefix(std::string v, unsigned short c, std::string m)
{
	msg_prefix = v + " " + std::to_string(c) + m + "\r\n";
}

void http::message::write_prefix(std::string m, std::string u   , std::string v)
{
	msg_prefix = m + " " + u + " " + v;
}
	
void http::message::write_header(std::string prefix, std::string context)
{
	msg_header.push_back(prefix + " : " + context + "\r\n");
}
#include <synapse/socket/socket.hpp>
#include <synapse/string/string_tools.hpp>

#include <variant>

namespace http
{
	class message
	{
	public:
		message() {}
		
		void prefix(std::string v, unsigned short c, std::string m);
		void prefix(std::string m, std::string u   , std::string v);
		void header(std::string prefix, std::string context);
	
	private:
		void parse (char* ctx, size_t len);
		
	public:
		friend network::socket_base& operator >> (network::socket_base& s,
											      message&				m)
		{
			if(m.msg_context != nullptr)
				delete[] m.msg_context;
			m.msg_header.clear() 	  ;
			
			char*   mb = new char[4096]; // Message Buffer
			int     mr = (int)s.recv((uint8_t*)mb, 4096); // Message Received

			if(mr > 0) {
				message msg		 ;
				msg.parse(mb, mr);
				
				m = msg;
			}

			return s;
		}
		
		friend network::socket_base& operator << (network::socket_base& s,
												  message& 			    m)
		{
			if(!m.msg_prefix.index()) {
				std::string_view m_pref = std::get<std::string_view>(m.msg_prefix);
				s.send((uint8_t*)m_pref.data(), 
					   			 m_pref.length());
			}
			else {
				std::string      m_pref = std::get<std::string>(m.msg_prefix);
				s.send((uint8_t*)m_pref.c_str(), 
					   			 m_pref.length());
			}
			
			for(auto& m_it : m.msg_header) {
				if(!m_it.index()) {
					std::string_view m_col = std::get<std::string_view>(m_it);
					s.send((uint8_t*)m_col.data(), 
						   			 m_col.length());
				}
				else {
					std::string 	 m_col = std::get<std::string>(m_it);
					s.send((uint8_t*)m_col.c_str(), 
						   			 m_col.length());
				}
			}
			
			s << "\r\n";
			s.send(m.msg_context, m.msg_ctx_size); return s;
		}
		
	public:
		using msg_col_type 	    = std::variant<std::string_view, std::string>;
		
		msg_col_type 			  msg_prefix;
		std::vector<msg_col_type> msg_header;
		
		uint8_t*				  msg_context  = nullptr;
		size_t					  msg_ctx_size			;
	};
}

void http::message::parse(char* raw, size_t len)
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

void http::message::prefix(std::string v, unsigned short c, std::string m)
{
	msg_prefix = v + " " + std::to_string(c) + m + "\r\n";
}

void http::message::prefix(std::string m, std::string u   , std::string v)
{
	msg_prefix = m + " " + u + " " + v;
}
	
void http::message::header(std::string prefix, std::string context)
{
	msg_header.push_back(prefix + ": " + context + "\r\n");
}
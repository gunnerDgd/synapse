#pragma once
#include <synapse/string/string_tools.hpp>
#include <synapse/inet/dns/dns_type.hpp>
#include <synapse/socket/udp/udp.hpp>

#include <variant>

namespace dns
{	
	struct header
	{
		struct		 flag_field
		{
			uint8_t  response  		 : 1;
			uint8_t  opcode	   		 : 4;
			uint8_t  auth	   		 : 1;
			uint8_t  truncated 		 : 1;
			uint8_t  recursion 		 : 1;
			
			uint8_t  avail_recursion : 1;
			uint8_t  reserved		 : 1;
			uint8_t  auth_answer	 : 1;
			uint8_t  non_authdata	 : 1;
			uint8_t  reply		     : 4;
		};

		uint16_t 	   transaction_id;
		union
		{
			flag_field dns_flags_field;
			uint16_t   dns_flags;
		};
		
		uint16_t 	   question	    ,
				 	   answer_rr	,
				 	   auth_rr	    ,
				 	   additional_rr;
	};
	
	struct query
	{	
		char* 	 name;
		struct   query_context
		{
			uint16_t dns_type,
				 	 dns_class;
		}			*context;
	};
	
	struct answer
	{
		char* 	 name;
		struct   answer_context
		{
			uint16_t dns_type,
				 	 dns_class;
		
			uint32_t ttl;
			uint16_t rd_length;
		}	   	    *context;
		
		char* 	 	 rdata;
	};
	
	struct dns_packet
	{
		header 									  *dns_header = nullptr;
		std::variant<query *, std::vector<query>>  dns_query  = nullptr;
		std::variant<answer*, std::vector<answer>> dns_answer = nullptr;
		
		char*	 	 							   dns_raw	  = nullptr;
	};
}

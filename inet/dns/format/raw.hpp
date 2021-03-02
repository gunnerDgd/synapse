#pragma once
#include <iostream>

namespace dns
{
namespace raw
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
		char* name; // only "Position" of the name field.
		struct query_context
		{
			uint16_t dns_type,
				 	 dns_class;
		}	*context;
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
}
}
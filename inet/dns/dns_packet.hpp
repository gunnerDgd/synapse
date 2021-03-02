#pragma once
#include <synapse/string/string_tools.hpp>
#include <synapse/inet/dns/dns_type.hpp>
#include <synapse/socket/udp/udp.hpp>

#include <variant>

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
	
	using v_dq = std::vector<query> ;
	using v_da = std::vector<answer>;
	
/*
"dns_query"

header::query* Type 	: When You Received the Packet.
std::vector<query> Type : When You Send the Packet.

"dns_answer"
header::answer* Type 	 : When You Received the Packet.
std::vector<answer> Type : When You Send the Packet.
*/

	struct dns_packet
	{
		dns_packet () 			  {}
		dns_packet (dns_packet& cp);
		~dns_packet()			   ;
		
		header 					   *dns_header = nullptr;
		char*	 	 				dns_raw	   = nullptr;
		
		std::variant<query *, v_dq> dns_query  = nullptr;
		std::variant<answer*, v_da> dns_answer = nullptr;
	};
}
}

// Copy Constructor of the DNS Packet.
dns::dns_packet::dns_packet  (dns_packet& cp)
{
	switch((dns_raw != nullptr) ? true : false)
	{
	case true:
	{
		cp.dns_header = dns_header; dns_header = nullptr;
		cp.dns_raw    = dns_raw   ; dns_raw    = nullptr;
		
		cp.dns_query  = dns_query ; dns_query  = nullptr;
		cp.dns_answer = dns_answer; dns_answer = nullptr; break;
	}
			
	case false:
	{
		cp.dns_header  = new header ;
		*cp.dns_header = *dns_header;
		
		cp.dns_query   = dns_query  ;
		cp.dns_answer  = dns_answer ; break;
	}
	}
}

// Destructor of the DNS Packet
dns::dns_packet::~dns_packet  ()
{
	// dns_raw != nullptr : The Packet is Received.
	// dns_raw == nullptr : The Packet is made by user.

	switch((dns_raw != nullptr) ? true : false)
	{
	case true:
	{
		query * dq;
		answer* da;
			
		if(dns_query.index()  == 0) {
			dq = std::get<dns::query *>(dns_query) ;
			
			if(!dq) {
				for(uint16_t q_it = 0 ; q_it < dns_header->question ; q_it++)
					delete[] dq[q_it].name;
				delete[] 	 dq;
			}
		}
		
		if(dns_answer.index() == 0) {
			da = std::get<dns::answer*>(dns_answer);
			
			if(!da) {
				for(uint16_t a_it = 0 ; a_it < dns_header->answer_rr
								  		 	 + dns_header->auth_rr  ; a_it++)
					delete[] da[a_it].name; // Name pointer was allocated "dynamically" by dns::parse class methods.
				delete[]     da;
			}
		}
		
		if(dns_raw != nullptr)
			delete[] dns_raw;
		break				;
	}
			
	case false:
	{
		if(dns_query.index()  == 1) {
			v_dq& dq = std::get<std::vector<query>> (dns_query) ;
			
			for(auto& q_it : dq) {
				delete[] q_it.name   ;
				delete   q_it.context;
			}
		}
		
		if(dns_answer.index() == 1) {
			v_da& da = std::get<std::vector<answer>>(dns_answer);
			
			for(auto& a_it : da) {
				delete[] a_it.name	 ;
				delete   a_it.context;
				delete[] a_it.rdata  ;
			}
		}
		
		if(dns_header != nullptr)
			delete dns_header;
		break			 	 ;
	}
	}
}

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

		uint16_t 		   transaction_id;
		union
		{
				flag_field dns_flags_field;
				uint16_t   dns_flags;
		};
		
		uint16_t 		 question,
				 		 answer_rr,
				 		 auth_rr,
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
	
	class dns_packet
	{
	public:
		dns_packet (char* raw, size_t size);
		dns_packet () {}
		
		~dns_packet() { if(dns_raw) delete[] dns_raw; }

	public:
		header 									  *dns_header = nullptr;
		std::variant<query *, std::vector<query>>  dns_query  = nullptr;
		std::variant<answer*, std::vector<answer>> dns_answer = nullptr;
		
		char*	 	 							   dns_raw	  = nullptr;
	};
	
	void dns_ntoh			 (header& dh);
	void dns_ntoh			 (query&  dq);
	void dns_ntoh			 (answer& da);
	
	void dns_hton			 (header& dh);
	void dns_hton			 (query&  dq);
	void dns_hton			 (answer& da);
	
	char* dns_name_compress  (char*  name);
	char* dns_name_decompress(char*  name);
	char* dns_name_decompress(char*& name, char* raw);
}

char* dns::dns_name_compress (char* name)
{
	std::string_view			  dname_v(name);
	std::vector<std::string_view> dname_col = string::split(dname_v, ".");
	
	char*						  dname_res = new char[dname_v.length() + 2],
								 *dname_ptr = dname_res;
	
	memset(dname_res, 0x00, dname_v.length() + 2);
	for(auto& n_it : dname_col) {
		*dname_ptr++ = n_it.length();
		strncpy(dname_ptr, n_it.data(), n_it.length()); dname_ptr += n_it.length();
	}
	
	return dname_res;
}

char* dns::dns_name_decompress(char* name)
{
	char* dname_res = new char[strlen(name)],
		* dname_ptr = dname_res;
	
	while(*name) {
		uint8_t clen = *name++;
		strncpy(dname_ptr, name, clen);
		
		name   	  += clen;
		dname_ptr += clen; 
		
		if(*name)
			*dname_ptr++ = '.';
	}
	
	return dname_res;
}

char* dns::dns_name_decompress(char*& name, char* raw)
{
	char* dc_res = new char[255],
	    * dc_ptr = dc_res;
	
	while(*name) {
		if((uint8_t)*name & 0xc0) // Compressed.
		{
			uint16_t dc_label  = (((uint8_t)name[0] & 0x3F) << 8) 
							   |   (uint8_t)name[1];			 name += 2;
			
			char*    comp_ptr  = raw + dc_label,
			    *	 label_ptr = dns_name_decompress(comp_ptr, raw);
			
			strncpy (dc_ptr, label_ptr, dc_label);
			return   dc_res;
		}
		
		uint8_t dc_len = *name++;
		strncpy(dc_ptr, name, dc_len);
		
		name   += dc_len;
		dc_ptr += dc_len; if(*name) *dc_ptr++ = '.';
	}   
	
	name++;
	return    dc_res;
}

void dns::dns_ntoh(header& dh)
{
	dh.transaction_id = ntohs(dh.transaction_id);
	dh.dns_flags	  = ntohs(dh.dns_flags);
	
	dh.question		  = ntohs(dh.question);
	dh.answer_rr	  = ntohs(dh.answer_rr);
	dh.auth_rr		  = ntohs(dh.auth_rr);
	dh.additional_rr  = ntohs(dh.additional_rr);
}

void dns::dns_ntoh(query&  dq)
{
	dq.context->dns_type  = ntohs(dq.context->dns_type);
	dq.context->dns_class = ntohs(dq.context->dns_class);
}

void dns::dns_ntoh(answer& da)
{
	da.context->dns_type  = ntohs(da.context->dns_type) ;
	da.context->dns_class = ntohs(da.context->dns_class);
	da.context->ttl		  = ntohl(da.context->ttl)		;
	da.context->rd_length = ntohs(da.context->rd_length);
}
	
void dns::dns_hton(header& dh)
{
	dh.transaction_id = htons(dh.transaction_id);
	dh.dns_flags	  = htons(dh.dns_flags)		;
	
	dh.question		  = htons(dh.question)	   ;
	dh.answer_rr	  = htons(dh.answer_rr)	   ;
	dh.auth_rr		  = htons(dh.auth_rr)	   ;
	dh.additional_rr  = htons(dh.additional_rr);
}

void dns::dns_hton(query&  dq)
{
	dq.context->dns_type  = htons(dq.context->dns_type) ;
	dq.context->dns_class = htons(dq.context->dns_class);
}

void dns::dns_hton(answer& da)
{
	da.context->dns_type  = htons(da.context->dns_type) ;
	da.context->dns_class = htons(da.context->dns_class);
	da.context->ttl		  = htonl(da.context->ttl)		;
	da.context->rd_length = htons(da.context->rd_length);
}

dns::dns_packet::dns_packet(char* raw, size_t size)
{
	dns_raw	   = raw;
	dns_header = (header*)raw ; raw += 12; // DNS Header Size.
	dns_ntoh     (*dns_header);
	
	size_t c_dq = dns_header->question,
		   c_da = dns_header->answer_rr + dns_header->auth_rr;
	
	if(c_dq) dns_query  = new query [c_dq];
	if(c_da) dns_answer = new answer[c_da];

	for(int q_it = 0 ; q_it < c_dq ; q_it++) {
		query& dq   = std::get<query*>(dns_query)[q_it];
	
		dq.name	    = dns_name_decompress(raw, dns_raw);
		dq.context  = (query::query_context*)raw; 
		
		dns_ntoh	  (dq);
		raw		   += 4;
	}

	for(int a_it = 0 ; a_it < c_da ; a_it++){
		answer& da = std::get<answer*>(dns_answer)[a_it];
		da.name	   = dns_name_decompress(raw, dns_raw);
		
		da.context = (answer::answer_context*)raw	  ;    raw += 10;
		da.rdata   = dns_name_decompress(raw, dns_raw); dns_ntoh(da);
	}
}
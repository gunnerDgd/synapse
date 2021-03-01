#include <synapse/inet/dns/dns_packet.hpp>

namespace dns
{
	class byte_order
	{
	public:
		static void ntoh (dns::header& dh);
		static void ntoh (dns::query&  dq);
		static void ntoh (dns::answer& da);
	
		static void hton (dns::header& dh);
		static void hton (dns::query&  dq);
		static void hton (dns::answer& da);
	};
	
	class compress
	{
	public:
		static char* dns_compress  (char*  name);
		static char* dns_decompress(char*& name, char* raw);
	};
	
	class parse
	{
	public:
		static std::pair<uint8_t*, size_t> parse_serialize(dns::dns_packet& p);
		static dns::dns_packet    		   parse_packet   (char* ctx, size_t len);
		static std::vector<char*> 		   parse_data     (dns::dns_packet& p);
	};
}

std::pair<uint8_t*, size_t> dns::parse::parse_serialize(dns::dns_packet& p)
{
	size_t   sp_size = 12;
	uint8_t *sp      = new uint8_t[2048],
			*dst_ptr = sp;
		
	memcpy(dst_ptr, (uint8_t*)p.dns_header, 12); dst_ptr += 12;
		
	if(p.dns_header->question) {
		std::vector<dns::query>& v_dq = std::get<std::vector<dns::query>>(p.dns_query);
		for(auto& q_it 	       : v_dq) {
			memcpy(dst_ptr,   q_it.name, strlen(q_it.name) + 1);
			dst_ptr += strlen(q_it.name) + 1;
			sp_size += strlen(q_it.name) + 1;
				
			memcpy(dst_ptr, q_it.context, 4);
			dst_ptr += 4;
			sp_size += 4;
		}
	}
		
	if(p.dns_header->answer_rr) {
		std::vector<dns::answer>& v_dq = std::get<std::vector<dns::answer>>(p.dns_answer);
		for(auto& q_it 	  		: v_dq) {
			memcpy(dst_ptr,   q_it.name, strlen(q_it.name) + 1);
			dst_ptr += strlen(q_it.name) + 1;
			sp_size += strlen(q_it.name) + 1;
				
			memcpy(dst_ptr, q_it.context, 4);
			dst_ptr += 4;
			sp_size += 4;
				
			memcpy(dst_ptr, q_it.rdata, q_it.context->rd_length);
			dst_ptr += q_it.context->rd_length;
			sp_size += q_it.context->rd_length;
		}
	}
		
	return std::make_pair(sp, sp_size);
}

char* dns::compress::dns_compress (char* name)
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


char* dns::compress::dns_decompress(char*& name, char* raw)
{
	char* dc_res = new char[255],
	    * dc_ptr = dc_res;
	
	while(*name) {
		if((uint8_t)*name & 0xc0) // Compressed.
		{
			uint16_t dc_label  = (((uint8_t)name[0] & 0x3F) << 8) 
							   |   (uint8_t)name[1];			 name += 2;
			
			char*    comp_ptr  = raw + dc_label,
			    *	 label_ptr = compress::dns_decompress(comp_ptr, raw);
			
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


void dns::byte_order::ntoh(header& dh)
{
	dh.transaction_id = ::ntohs(dh.transaction_id);
	dh.dns_flags	  = ::ntohs(dh.dns_flags);
	
	dh.question		  = ::ntohs(dh.question);
	dh.answer_rr	  = ::ntohs(dh.answer_rr);
	dh.auth_rr		  = ::ntohs(dh.auth_rr);
	dh.additional_rr  = ::ntohs(dh.additional_rr);
}

void dns::byte_order::ntoh(query&  dq)
{
	dq.context->dns_type  = ::ntohs(dq.context->dns_type);
	dq.context->dns_class = ::ntohs(dq.context->dns_class);
}

void dns::byte_order::ntoh(answer& da)
{
	da.context->dns_type  = ::ntohs(da.context->dns_type) ;
	da.context->dns_class = ::ntohs(da.context->dns_class);
	da.context->ttl		  = ::ntohl(da.context->ttl)		;
	da.context->rd_length = ::ntohs(da.context->rd_length);
}
	
void dns::byte_order::hton(header& dh)
{
	dh.transaction_id = ::htons(dh.transaction_id);
	dh.dns_flags	  = ::htons(dh.dns_flags)		;
	
	dh.question		  = ::htons(dh.question)	   ;
	dh.answer_rr	  = ::htons(dh.answer_rr)	   ;
	dh.auth_rr		  = ::htons(dh.auth_rr)	   ;
	dh.additional_rr  = ::htons(dh.additional_rr);
}

void dns::byte_order::hton(query&  dq)
{
	dq.context->dns_type  = ::htons(dq.context->dns_type) ;
	dq.context->dns_class = ::htons(dq.context->dns_class);
}

void dns::byte_order::hton(answer& da)
{
	da.context->dns_type  = ::htons(da.context->dns_type) ;
	da.context->dns_class = ::htons(da.context->dns_class);
	da.context->ttl		  = ::htonl(da.context->ttl)	  ;
	da.context->rd_length = ::htons(da.context->rd_length);
}

std::vector<char*> dns::parse::parse_data  (dns::dns_packet& p)
{
	std::vector<char*> r_res;
	dns::answer		 * r_ans = std::get<dns::answer*>(p.dns_answer);
	
	for(int a_it = 0 ; a_it < (p.dns_header->answer_rr
	   			 			+  p.dns_header->auth_rr)  ; a_it++) {
		switch(r_ans[a_it].context->dns_type)
		{
			case dns::dns_type::ns_t_a:
			{
				char*   a_ip = new char[16];
				sprintf(a_ip, "%u.%u.%u.%u", 
					   (uint8_t)r_ans[a_it].rdata[0],
					   (uint8_t)r_ans[a_it].rdata[1],
					   (uint8_t)r_ans[a_it].rdata[2],
					   (uint8_t)r_ans[a_it].rdata[3]); r_res.push_back(a_ip); break;
			}
			
			case dns::dns_type::ns_t_ns   :
			case dns::dns_type::ns_t_cname:
			{
				char* a_ptr = r_ans[a_it].rdata;
				r_res.push_back(dns::compress::dns_decompress(a_ptr, p.dns_raw));
				
				break;
			}
		}
	}
	
	return r_res;
}

dns::dns_packet    dns::parse::parse_packet(char* raw, size_t size)
{
	dns::dns_packet dp;
	
	dp.dns_raw	  = 		 raw;
	dp.dns_header = (header*)raw 	; raw += 12; // DNS Header Size.
	byte_order::ntoh(*dp.dns_header);
	
	size_t c_dq   = dp.dns_header->question,
		   c_da   = dp.dns_header->answer_rr + dp.dns_header->auth_rr;
	
	if(c_dq) dp.dns_query  = new query [c_dq];
	if(c_da) dp.dns_answer = new answer[c_da];

	for(int q_it = 0 ; q_it < c_dq ; q_it++) {
		query& dq   = std::get<query*>(dp.dns_query)[q_it];
	
		dq.name	    = compress::dns_decompress(raw, dp.dns_raw);
		dq.context  = (query::query_context*)  raw;
		
		byte_order::ntoh(dq);
		raw		   		+= 4;
	}

	for(int a_it = 0 ; a_it < c_da ; a_it++){
		answer& da = std::get<answer*>(dp.dns_answer)[a_it];
		
		da.name	   = compress::dns_decompress(raw, dp.dns_raw) ;
		da.context = (answer::answer_context*)raw;    raw += 10;
		
		da.rdata   = raw; byte_order::ntoh(da);
		raw 	  += da.context->rd_length    ;
	}
	
	return dp;
}
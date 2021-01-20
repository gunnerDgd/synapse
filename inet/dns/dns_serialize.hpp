#pragma once
#include <synapse/inet/dns/dns_packet.hpp>

namespace dns
{
	std::pair<uint8_t*, size_t> dns_serialize(dns_packet& p)
	{
		size_t   sp_size = 12;
		uint8_t *sp      = new uint8_t[2048],
				*dst_ptr = sp;
		
		memcpy(dst_ptr, (uint8_t*)p.dns_header, 12); dst_ptr += 12;
		
		if(p.dns_header->question) {
			std::vector<query>& v_dq = std::get<std::vector<query>>(p.dns_query);
			for(auto& q_it 	  : v_dq) {
				memcpy(dst_ptr,   q_it.name, strlen(q_it.name) + 1);
				dst_ptr += strlen(q_it.name) + 1;
				sp_size += strlen(q_it.name) + 1;
				
				memcpy(dst_ptr, q_it.context, 4);
				dst_ptr += 4;
				sp_size += 4;
			}
		}
		
		if(p.dns_header->answer_rr) {
			std::vector<answer>& v_dq = std::get<std::vector<answer>>(p.dns_answer);
			
			for(auto& q_it 	  : v_dq) {
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
}
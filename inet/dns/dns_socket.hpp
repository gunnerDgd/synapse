#pragma once
#include <synapse/inet/dns/dns_serialize.hpp>

#include <vector>
#include <cstring>
#include <cstdlib>
#include <ctime>

namespace dns
{
	class dns_socket : public network::udp
	{
	public:
		dns_socket(const char* dns_ip, uint16_t dns_port)
			: network::udp(dns_ip, dns_port) {}
		
	public:
		void write_header(uint16_t 			 d_tid,
						  header::flag_field d_flag,
						  uint16_t 			 d_qcount,
						  uint16_t 			 d_acount,
						  uint16_t 			 d_authcount = 0,
						  uint16_t 			 d_addcount  = 0);
						  
		void write_query (char* 	q_name,
						  dns_type  q_type,
						  dns_class q_class);
						  
		void write_answer(char*		a_name,
						  dns_type  a_type,
						  dns_class a_class,
						  uint32_t  a_ttl,
						  char*		a_rdata,
						  uint16_t  a_rlength);
						  
	public:
		void 			 dns_send();
		dns::dns_packet* dns_recv();
		
	private:
		dns_packet		      d_packet;
		network::socket_base *d_socket;
	};
}

void 		dns::dns_socket::dns_send()
{
	auto sp_pair = dns::dns_serialize(d_packet);	
	this  ->send(sp_pair.first, sp_pair.second); delete[] sp_pair.first;
}

dns::dns_packet* dns::dns_socket::dns_recv()
{
	char  *d_buf  = new char[4096];
	int    d_recv = (int)this->recv((uint8_t*)d_buf, 4096);
	
	if(d_recv <= 0)
		return nullptr;
	else
		return new dns::dns_packet(d_buf, d_recv);
}
		

void dns::dns_socket::write_header(uint16_t 			 d_tid,
				  				   header::flag_field    d_flag,
				  				   uint16_t 			 d_qcount,
				  				   uint16_t 			 d_acount,
				  				   uint16_t 			 d_authcount ,
				  				   uint16_t 			 d_addcount)
{
	d_packet.dns_header					 = new header;
	
	d_packet.dns_header->transaction_id  = d_tid;
	d_packet.dns_header->dns_flags_field = d_flag;
	
	d_packet.dns_header->question		 = d_qcount;
	d_packet.dns_header->answer_rr		 = d_acount;
	d_packet.dns_header->auth_rr		 = d_authcount;
	d_packet.dns_header->additional_rr	 = d_addcount;
	
	dns::dns_hton(*d_packet.dns_header);
}
						  
void dns::dns_socket::write_query (char* 	 q_name,
				  				   dns_type  q_type,
				  				   dns_class q_class)
{
	d_packet.dns_query		 = std::vector<query>();
	std::vector<query>& v_dq = std::get<std::vector<query>>(d_packet.dns_query);
	query				dq;
	
	dq.name    = dns::dns_name_compress(q_name);
	dq.context = new query::query_context;

	dq.context->dns_type  = q_type;
	dq.context->dns_class = q_class;
	
	dns::dns_hton (dq);
	v_dq.push_back(dq);
}

void dns::dns_socket::write_answer(char*	 a_name ,
				  				   dns_type  a_type ,
				  				   dns_class a_class,
				  				   uint32_t  a_ttl  ,
				  				   char*	 a_rdata,
				  				   uint16_t  a_rlength)
{
	d_packet.dns_answer		  = std::vector<answer>();
	std::vector<answer>& v_da = std::get<std::vector<answer>>(d_packet.dns_answer);
	answer				 da;
	
	da.name    = dns::dns_name_compress(a_name);
	da.context = new answer::answer_context;
	
	da.context->dns_type  = a_type;
	da.context->dns_class = a_class;
	da.context->ttl		  = a_ttl;
	da.context->rd_length = a_rlength;
	da.rdata			  = a_rdata;
	
	dns::dns_hton (da);
	v_da.push_back(da);
}
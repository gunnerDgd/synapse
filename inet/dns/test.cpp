#include <synapse/inet/dns/dns_socket.hpp>
#include <synapse/inet/dns/dns_serialize.hpp>

#include <ctime>

int main()
{
	dns::dns_socket dsock  ("168.126.63.1", 53);
	dns::header::flag_field d_ff = {0x00, };
	
	d_ff.response  = 0;
	d_ff.opcode    = dns::dns_opcode::ns_o_query;
	d_ff.recursion = 1;

	dsock.write_header(0x1032,
					   d_ff,
					   1, 0, 0, 0);
	dsock.write_query("www.naver.com", 
					  dns::dns_type::ns_t_txt, 
					  dns::dns_class::ns_c_in);
	
	dsock.dns_send();
	dns::dns_packet* packet = dsock.dns_recv();
	
	for(int i = 0 ; i < packet->dns_header->auth_rr ; i++)
	std::cout << std::get<dns::answer*>(packet->dns_answer)[i].rdata << std::endl;
}
#pragma once
#include <synapse/synapse.hpp>
#include <synapse/inet/dns/packet/dns_type.hpp>

#include <vector>

namespace synapse {
namespace network {
namespace dns     {
namespace packet  {

    struct flag
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

    class header
    {
    public:
        header(uint16_t                            h_trans_id, 
               synapse::network::dns::packet::flag h_flag    ,
               uint16_t                            h_qc   = 0, 
               uint16_t                            h_ac   = 0, 
               uint16_t                            h_auc  = 0, 
               uint16_t                            h_adc  = 0)
            : transaction_id  (h_trans_id),
              flag            (h_flag),
              query_count     (h_qc),
              answer_count    (h_ac),
              authorized_count(h_auc),
              additional_count(h_adc) { }    

        uint16_t   transaction_id;
        union
        {
            synapse::network::dns::packet::flag flag    ;
            uint16_t                            flag_int;
        };

        uint16_t   query_count     ,
                   answer_count    ,
                   authorized_count,
                   additional_count;
    };

    class query
    {
    public:
        query(std::string                      q_name,
              synapse::network::dns::dns_type  q_type  = synapse::network::dns::dns_type::a  ,
              synapse::network::dns::dns_class q_class = synapse::network::dns::dns_class::in)
            : query_name (std::move(q_name)),
              query_type ((uint16_t)q_type) ,
              query_class((uint16_t)q_class) { }

    public:
        std::string query_name ;
        uint16_t    query_type ,
                    query_class;
    };

    class answer
    {
    public:
        answer(std::string                      a_name   ,
               void*                            a_ctx    ,
               uint16_t                         a_ctx_len,
               synapse::network::dns::dns_type  a_type  = synapse::network::dns::dns_type::a  ,
               synapse::network::dns::dns_class a_class = synapse::network::dns::dns_class::in,
               uint32_t                         a_ttl   = 150)
        : answer_name  (std::move(a_name)),
          answer_type  ((uint16_t)a_type) ,
          answer_class ((uint16_t)a_class),
          answer_ttl   (a_ttl)            ,
          answer_length(a_ctx_len)        ,
          answer_data  (a_ctx)            { }


    public:
        std::string answer_name;
        uint16_t    answer_type,
                    answer_class;

        uint32_t    answer_ttl;
        uint16_t    answer_length;

        void*       answer_data;
    };

    class packet
    {
        synapse::network::dns::packet::header              dns_header;
        std::vector<synapse::network::dns::packet::query>  dns_query ;
        std::vector<synapse::network::dns::packet::answer> dns_answer;
    };

}
}
}
}
#pragma once
#include <synapse/synapse.hpp>

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
        std::string query_name ;
        uint16_t    query_type ,
                    query_class;
    };

    class answer
    {
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
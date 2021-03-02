#pragma once
#include <synapse/inet/dns/format/raw.hpp>

namespace dns
{
namespace parsed
{
    class header
    {
    public:
        header(raw::header& r);
        header()              {}

    public:
        uint16_t                transaction_id;
        raw::header::flag_field flag          ;

        uint16_t                question_count  ,
                                answer_count    ,
                                authorize_count ,
                                additional_count;
    };

    class query
    {
    public:
        query(raw::query& r);
        query()             {}

    public:
        std::string name;
        uint16_t    type,
				 	class;
    };

    class answer
    {
    public:
        answer(raw::answer& r);
        answer()              {}
        
    public:
        std::string name;
		uint16_t    type,
				    class;
		
		uint32_t ttl;
		uint16_t data_length;
    };
}
}
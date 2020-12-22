#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <set>

#include <synapse/memory/view.hpp>

namespace format
{
    using string_list	   = std::vector<std::string>;
	using string_view_list = std::vector<memory::view<char>>;
    using offset_list	   = std::set<size_t>;

    class string
    {
        public:
            static void             trim	  (std::string& target, std::string delim);
            
			static string_list		split	  (std::string  target, std::string delim);
			static string_view_list split_view(std::string& target, std::string delim);

			static offset_list		find	  (std::string& target, std::string delim);
    };
}
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <set>

namespace format
{
    using string_list = std::vector<std::string>;
    using offset_list = std::set<size_t>;

    class string
    {
        public:
            static void        trim  (std::string& target, std::string delim);
            static string_list split (std::string  target, std::string delim);
            static offset_list find  (std::string& target, std::string delim);
    };
}
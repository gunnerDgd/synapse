#include <iostream>
#include <string>

namespace format
{
    using string_list = std::list<std::string>;
    using offset_list = std::set<size_t>;

    class string
    {
        public:
            static void        trim  (std::string& target, std::string delim);
            static string_list split (std::string& target, std::string delim);
            static offset_list find  (std::string& target, std::string delim);
    }
}
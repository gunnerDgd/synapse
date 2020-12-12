#include <iostream>
#include <string>

namespace format
{
    class string
    {
        static void                   trim  (std::string& target, std::string delim);
        static std::list<std::string> split (std::string& target, std::string delim);
        static std::set<size_t>       find  (std::string& target, std::string delim);
    }
}
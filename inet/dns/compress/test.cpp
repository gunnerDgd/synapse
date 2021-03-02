#include "compress.hpp"

int main()
{
    std::string n = dns::name::hton("www.naver.com");

    std::cout << dns::name::ntoh((char*)n.c_str(), nullptr) << std::endl;

    return 0;
}
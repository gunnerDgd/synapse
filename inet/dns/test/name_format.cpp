#include <synapse/inet/dns/format/name_format.hpp>
#include <iostream>

int main()
{
    std::string test_string = "www.naver.com";
    uint16_t    cvt_length;
    
    char*       cvt_string = synapse::network::dns::name_format::host_to_network(test_string, cvt_length);
    for(int i = 0 ; i < cvt_length ; i++)
        std::cout << std::hex << (int)cvt_string[i] << " ";

    return 0;
}
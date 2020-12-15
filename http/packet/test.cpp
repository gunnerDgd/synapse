#include "request.hpp"

int main()
{
    char*       test = new char[4096];
    const char* test_http = "GET / HTTP/1.1\r\nHost: www.google.co.kr\r\nAccept: text/html\r\n\r\nHello World";

    strncpy(test, test_http, strlen(test_http));

    http::request t_req((char*)test);

    std::cout << t_req.r_method  << std::endl << t_req.r_url << std::endl << t_req.r_version << std::endl;
    std::cout << t_req.p_context << std::endl;
}
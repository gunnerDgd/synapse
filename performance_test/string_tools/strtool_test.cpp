#include <synapse/string/string_tools.hpp>
#include <synapse/debug_tool/runtime_timer.hpp>

#include <cstring>
#include <xmmintrin.h>

int main()
{
    synapse::string::str_vec test_cnt;
    {
    synapse::debug_tool::runtime_timer dbg_timer;

    char test_str[] = "www.google.com.ss";
    _mm_prefetch(test_str, _MM_HINT_T0);

    char* test_ptr = std::strtok(test_str, ".");
    while(test_ptr) {
        std::printf("%s\n", test_ptr);
        test_ptr = std::strtok(nullptr, ".");
    }
    }

}
#include <iostream>
#include <cstdlib>
#include <cstring>

#include <chrono>
#include <xmmintrin.h>

int main()
{
    alignas(64) char test_src[64] = {'a', };
    char test_dst[64] = {0x00, };
    
    auto start = std::chrono::high_resolution_clock::now();

    _mm_prefetch(test_src, _MM_HINT_T0);
    memcpy(test_dst, test_src, 64);

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << (end - start).count() << std::endl;

    return 0;
}
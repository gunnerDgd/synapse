#include <synapse/memory/segment/segment.hpp>
#include <chrono>

int main()
{
    char test_char[12];
    memory::segment _seg;
    _seg.add_segment(6);
    _seg.add_segment(7);

    auto start = std::chrono::high_resolution_clock::now();
    _seg.copy_memory((uint8_t*)"Hello World", 11);
    auto end   = std::chrono::high_resolution_clock::now();

    std::cout << (end - start).count() << std::endl;

    
    memcpy(test_char, "Hello World", 11);


    start = std::chrono::high_resolution_clock::now();
    for(int i = 0 ; i < 11 ; i++)
        std::cout << (char)_seg[i] << std::endl;
    end   = std::chrono::high_resolution_clock::now();
    std::cout << (end - start).count() << std::endl;

}
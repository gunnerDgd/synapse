#include <iostream>

struct comp_field
{
    uint16_t compression_offset : 14;
    uint16_t compression_flag   : 2;
};

int main()
{
    uint16_t test =  (0xc0 << 8) | 0x15;
    comp_field* test_comp = (comp_field*)&test;
    std::cout << test_comp->compression_flag << std::endl;
}
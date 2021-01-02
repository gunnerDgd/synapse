#include <iostream>
#include <synapse/memory/memory_object/type/page.hpp>

int main()
{
    memory::page p_test(12);
    p_test.allocate();
    
    std::cout << p_test.native_pointer() << std::endl;
    
    return 0;
}
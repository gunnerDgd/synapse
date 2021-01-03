#include <iostream>
#include <synapse/memory/memory_object/type/view.hpp>
#include <synapse/memory/memory_object/type/access.hpp>
#include <synapse/memory/memory_object/type/heap.hpp>

int main()
{
    memory::heap         a(12);
    memory::access<char> b(a, 0, 12);
    b = "Hello C++";

    memory::view<char> a_view(a, 0, 9);

    std::cout << (a == a_view) <<std::endl;
    for(int i = 0 ; i < a_view.size() ; i++)
        std::cout << a_view[i];

    return 0;
}
#include <synapse/memory/mobject.hpp>
#include <cstring>

int main()
{
    memory::memory_object<int> t_mobj(5);
    strcpy((char*)t_mobj.native_pointer(), "Hello World 2");

    memory::memory_object<char> t_cobj = t_mobj.change_view<char>(20);

    for(int i = 0 ; i < 20 ; i++)
        std::cout << t_cobj[i];

    std::cout << std::endl;

    return 0;
}
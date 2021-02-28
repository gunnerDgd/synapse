#include <iostream>
#include <typeinfo>

    template <typename T>
    struct func_type { };

    template <typename _out, typename... _in>
    struct func_type<_out(_in...)>
    {
        using param_type  = _in...;
        using return_type = _out  ;
    };

int test(int a)
{
    std::cout << a << std::endl;
    return 3;
}

int main()
{
    std::cout << typeid(func_type<int(int)>::return_type).name() << std::endl;
}
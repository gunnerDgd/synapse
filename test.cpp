#include <iostream>

template <bool i, typename t_True, typename t_False>
struct compile_if {};

template <typename t_True, typename t_False>
struct compile_if<true, t_True, t_False>
{
    using Type = t_True;
};

template <typename t_True, typename t_False>
struct compile_if<false, t_True, t_False>
{
    using Type = t_False;
};



int main()
{
    for(int i = 0; i < 5 ; i++)
        std::cout << i << std::endl;
}
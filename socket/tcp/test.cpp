#include <iostream>
#include <variant>

std::variant<int, float> ret_val(bool what)
{
    if(what)
        return 3;
    else
        return 3.14;
}

int main()
{
    std::cout << ret_val(true)  << std::endl;
    std::cout << ret_val(false) << std::endl;
}
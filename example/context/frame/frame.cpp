#include <synapse/context/stack/stack_operation.hpp>

class test_class
{
public:
    int a;
    float b;
    double c;
};

int main()
{
    test_class a, *ptr_a;
    a.a = 3;
    a.b = 3.0;
    a.c = 3.0;

    stack::push_stack(&a, ptr_a);
    std::cout << ptr_a->a << " "
              << ptr_a->b << " "
              << ptr_a->c << std::endl;
    
    return 0;
}
#include <synapse/context/enumerator.hpp>

void test_1(frame::enumerator&, int&, int&);
void test_2(frame::enumerator&, int&, int&);

int main()
{
    frame::enumerator e;
    int a = 1, b = 2;

    e.start(test_1, a, b);

    std::cout << "The End\n";
}

void test_1(frame::enumerator& e, int& a, int& b)
{
    std::cout << "Hello Test 1 # 1\n"; a++, b++;
    std::cout << a << " " << b << " " << std::endl;
    
    e.start(test_2, a, b);

    std::cout << "Hello Test 1 # 2\n"; a++, b++;
    std::cout << a << " " << b << " " << std::endl;
    
    e.switch_to(test_2);
}

void test_2(frame::enumerator& e, int& a, int& b)
{
    std::cout << "Hello Test 2 # 1\n"; a++, b++;
    std::cout << a << " " << b << " " << std::endl;

    e.switch_to(test_1);

    std::cout << "Hello Test 2 # 2\n"; a++, b++;
    std::cout << a << " " << b << " " << std::endl;
    
    e.switch_to(0);
}
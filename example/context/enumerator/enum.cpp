#include <synapse/context/launch.hpp>

void test_1(frame::enumerator&, int&, int&);
void test_2(frame::enumerator&, int&, int&);

int main()
{
    frame::enumerator e;
    int a = 1, b = 2;

    frame::launcher<void(int&, int&)> l1(e, test_1, a, b);
    l1.launch();

    e.start(test_1, a, b);
    e.end  (test_1);

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
    e.end      (test_2);
    
    e.switch_to();
}

void test_2(frame::enumerator& e, int& a, int& b)
{
    std::cout << "Hello Test 2 # 1\n"; a++, b++;
    std::cout << a << " " << b << " " << std::endl;

    e.switch_to(test_1);

    std::cout << "Hello Test 2 # 2\n"; a++, b++;
    std::cout << a << " " << b << " " << std::endl;
    
    e.switch_to(test_1);
}
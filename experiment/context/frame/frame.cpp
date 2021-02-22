#include <synapse/context/frame/frame.hpp>

frame::frame* ca, *ce;

void test(int a, int b, int* c)
{
    std::cout << "#1 : " << a << " " << b << " " << *c << std::endl;
    *c += 1;
    frame::switch_frame(ce, ca);
    std::cout << "#2 : " << a << " " << b << " " << *c << std::endl;
    frame::switch_frame(ca);
}

int main()
{
    int a = 3, *b = &a;
    ca = new frame::frame();
    ce = new frame::frame(64 * 1024);

    std::cout << "Main #1\n";
    frame::switch_frame(ca, ce, test, 1, 2, b);
    
    std::cout << "Main #2\n";
    frame::switch_frame(ca, ce);

    return 0;
}
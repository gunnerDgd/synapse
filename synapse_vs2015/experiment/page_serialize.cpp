#include <iostream>
#include <Windows.h>

void test(const char* test_char)
{
	std::cout << typeid(test_char).name() << std::endl;
}

int main()
{
	test("Hello World");
	while (true) { Sleep(1000); }
}
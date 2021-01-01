#include <iostream>
#include <Windows.h>
#include <synapse/memory/view.hpp>
#include <chrono>
//#include <synapse/type/class/type_class.hpp>

template <typename T>
class test_item
{
public:
	T item;
};

template <>
class test_item<void> {};

template <typename... Types>
class test_tuple;

template <>
class test_tuple<> {};

template <typename T, typename... Types>
class test_tuple<T, Types...> : public test_tuple<Types...>
{
public:
	test_tuple() { std::cout << typeid(*this).name() << std::endl; }
	T item;
};

template <typename T, typename... Types>
void extract_tuple(test_tuple<T, Types...>& a)
{
	a.item = 2;
	std::cout << a.item << std::endl;
	tuple_for((test_tuple<Types...>)a);
}

template <typename... Types>
void tuple_for(test_tuple<Types...>& a)
{
	extract_tuple(a);
}


template <>
void tuple_for(test_tuple<>& a) {}

int main()
{
	char test[16] = "Hello World";
	memory::view<char> a(test, 5);

	auto b = a.view_pointer();
	b = nullptr;

	while (true) { Sleep(1000); }
}
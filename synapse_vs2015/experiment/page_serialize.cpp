#include <synapse/memory/mpool.hpp>

#include <iostream>
#include <thread>
#include <list>
#include <chrono>

#include <Windows.h>

struct dummy { int _dummy[1024]; };

int main()
{
	memory::memory_pool<dummy, 1000> a;
	memory::memory_block<dummy>*	 b;
	dummy*						     c;

	auto start = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < 1000; i++)
		b = a.acquire();

	auto end   = std::chrono::high_resolution_clock::now();

	std::cout << (end - start).count() << std::endl;

	start = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < 1000; i++)
		c = new dummy;

	end = std::chrono::high_resolution_clock::now();

	std::cout << (end - start).count() << std::endl;

	while (true) { Sleep(1000); }
}
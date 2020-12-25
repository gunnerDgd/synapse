#include <synapse/memory/mpool.hpp>
#include <Windows.h>

#include <chrono>
#include <iostream>

struct dummy { char test[2048]; };

int main()
{
	memory::memory_pool<dummy, 1024> memory_pool(memory::mpool_location::page);
	dummy* test[1024];

	auto start = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < 1024; i++)
		test[i] = memory_pool.acquire();
	
	auto end = std::chrono::high_resolution_clock::now();

	std::cout << (end - start).count() << std::endl;

	start = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < 1024; i++)
		test[i] = new dummy;

	end = std::chrono::high_resolution_clock::now();

	std::cout << (end - start).count() << std::endl;

	while (true) { Sleep(4000); }
}
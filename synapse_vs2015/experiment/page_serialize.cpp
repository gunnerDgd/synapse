#include <synapse/memory/page.hpp>
#include <synapse/memory/heap.hpp>
#include <synapse/format/string.hpp>

#include <ctime>

#define OFFSET 4096*1024

std::string rand_string()
{
	std::string _res;

	for (int i = 0; i < 100000 ; i++)
		_res += "HelloWorld#" + std::to_string(i) + " ";

	return _res;
}

int main()
{
	std::string s = rand_string();
	
	clock_t split_view_start = clock();
	auto sp_v = format::string::split_view(s, " ");
	clock_t split_view_end   = clock();

	clock_t split_start = clock();
	auto sp = format::string::split(s, " ");
	clock_t split_end   = clock();

	std::cout << "Split View : " << split_view_end - split_view_start << std::endl;
	std::cout << "Split : " << split_end - split_start << std::endl;

	while (true) { Sleep(1000); }
}
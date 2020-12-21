#include <synapse/disk/file/file.hpp>

int main()
{
    disk::file t_file("test.txt", disk::file::access_mode::all); t_file.mode(stream::stream::stream_mode::async);
    t_file.on_error = [&](disk::file* _f, disk::file::error_code _e) { std::cout << (int)_e << std::endl; };

    char test[12] = "Hello World";

    t_file.write((uint8_t*)test, 11);
	t_file.close();
}
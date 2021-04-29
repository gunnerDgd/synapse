#include <synapse/memory/io_map/io_map.hpp>

int main()
{
    synapse::disk::file tst_file("test.txt");
    
    tst_file.open(synapse::disk::access_mode::all, synapse::disk::open_mode::open);
    tst_file << "Hello World";

    std::cout << tst_file.size() << std::endl;
    synapse::memory::io_map tst_iomap(tst_file);
    
    std::cout << tst_iomap.view()                  << std::endl;
    std::cout << (char*)tst_iomap.memory_pointer() << std::endl;
}
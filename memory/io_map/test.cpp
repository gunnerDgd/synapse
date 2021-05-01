#include <synapse/memory/io_map/io_map.hpp>
#include <cstdlib>
#include <cstring>

int main()
{
    synapse::disk::file tst_file("test.txt");
    tst_file.open  (synapse::disk::access_mode::all, synapse::disk::open_mode::open);
    tst_file.resize(12);

    synapse::memory::io_map tst_iomap(tst_file);
    void* tst_ptr         = tst_iomap.view();

    strcpy      ((char*)tst_ptr, "Hello World");
    std::cout << (char*)tst_ptr << std::endl;

    tst_iomap.sync  ();
    tst_iomap.unview();
    tst_file .close ();
}
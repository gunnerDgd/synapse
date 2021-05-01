#include <synapse/disk/file/file.hpp>

int main()
{
    synapse::disk::file f("test.txt");
    std::cout << f.open() << std::endl;

    f << "Hello World";
    f.close();
}
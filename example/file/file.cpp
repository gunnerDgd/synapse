#include <synapse/disk/file/file.hpp>
#include <thread>

using namespace synapse;
int main()
{
    disk::file f(stream::io_mode::non_block);
    std::cout << f.open("./test.txt", 
                 disk::access_mode::all,
                 disk::open_mode::create,
                 stream::io_mode::non_block) << std::endl;

    uint64_t* test = new uint64_t[1024 * 1024 * 8]; // 64 MB
    for(uint64_t i = 0 ; i < 1024 * 1024 * 8 ; i++) test[i] = i;

    auto wr_func = [&]() { for(int i = 0 ; i < 5 ; i++) std::cout << f.write((uint8_t*)test, 1024 * 1024 * 8 * 8) << std::endl; };
    std::thread t1(wr_func), t2(wr_func), t3(wr_func), t4(wr_func);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
}
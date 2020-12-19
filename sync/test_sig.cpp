#include <synapse/sync/signal.hpp>
#include <thread>

int main()
{
    synchronous::signal<int> test_sig;
    std::thread t1([&]()
    {
        int d_value = 0;
        while(true)
        {
            usleep(2000*1000);
            test_sig = d_value++;
        }
    }),
    t2([&]()
    {
        while(true)
        {
            std::cout << test_sig.wait() << std::endl;
        }
    });

    t1.join();
    t2.join();
}
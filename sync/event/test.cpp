#include <synapse/sync/event/event.hpp>
#include <thread>

int main()
{
    synapse::synchronize::event evt;
    std::thread t1([&]() 
    { 
        for(int i = 0 ; i < 5 ; i++)
        {
            evt.wait();
            std::cout << "Called #" << i << std::endl;
        }
    }),
    t2([&]()
    {
        for(int i = 0 ; i < 5 ; i++)
        {
            usleep(1000 * 2000);
            evt.alert();
        }
    });

    t1.join();
    t2.join();
}
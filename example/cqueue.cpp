#include <synapse/lockfree/queue.hpp>
#include <synapse/sync/event.hpp>
#include <thread>

int main()
{
    lockfree::cqueue<int, 1024> cq_test;
    synchronous::event cq_evt;

    auto tst_func = [&](int start)
    {
        cq_evt.wait();
        
        for(int i = start ; start < start + 250 ; start++)
            cq_test.enqueue(i);
    };

    std::thread t1(tst_func, 0), t2(tst_func, 251), t3(tst_func, 501), t4(tst_func, 751);

    cq_evt.alert();
    t1.join(); t2.join(); t3.join(); t4.join();

    size_t sum = 0;
    for(int i = 0 ; i < 1000 ; i++)
        sum += *cq_test.dequeue();

    std::cout << sum << std::endl;
}
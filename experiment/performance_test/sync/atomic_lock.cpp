#include <synapse/sync/atomic_lock/atomic_semaphore.hpp>
#include <synapse/debug_tool/runtime_timer.hpp>

#include <iostream>
#include <vector>
#include <mutex>
#include <thread>

int main()
{
    synapse::synchronize::atomic_lock::atomic_semaphore sl(2);
    std::string ss = "test";
    std::mutex sm;

    auto th = [&]()
    {
        int i = 1024;
        while(i-- > 0)
        {
            //sl.acquire();
            sm.lock();
            ss = "Test_2";
            sm.unlock();
            //sl.release();
        }
    };
    synapse::debug_tool::runtime_timer st;
    std::vector<std::thread> sv;

    for(int i = 0 ; i < 4 ; i++)
        sv.push_back(std::thread(th));

    for(auto& ti : sv) ti.join();
}
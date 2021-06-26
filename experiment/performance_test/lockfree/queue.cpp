#include <synapse/debug_tool/runtime_timer.hpp>
#include <synapse/lockfree/queue/queue.hpp>

#include <thread>

int main()
{
    synapse::debug_tool::runtime_timer                 rt_timer  ;
    synapse::lockfree::queue<std::string, 1024 * 1024> tst_queue ;
    std::vector<std::string>                           tst_vector;

    auto th_func = [&]()
    {
        for()
    }
    std::thread
}
#include <synapse/thread/thread_pool/thread_pool.hpp>

int test_func(int a, int b) { return a + b; }

int main()
{
    synapse::memory::vmem wk_mem(8 * 1024);
    synapse::thread::work<int(int, int)> wk_test(test_func, 1, 2);
    synapse::thread::thread_pool wk_tp(wk_mem, 1);

    wk_tp.enqueue_work(wk_test);
    wk_tp.start_thread_pool();
    wk_tp.end_thread_pool();
}
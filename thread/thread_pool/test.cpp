#include <synapse/thread/thread_pool/thread_pool.hpp>

int test_func(int a, int b) { return a + b; }

int main()
{
    synapse::thread::work<int(int, int)> wk_test(test_func, 1, 2);
}
#include <synapse/thread/pool/thread_pool.hpp>

class test_tsk : public thread::thread_task
{
    public:
        void launch() override
        {
            for(int i = 0 ; i < 5 ; i++)
                std::cout << "Hello World" << std::endl;
        }
};

int main()
{
    thread::thread_pool<4> t_pool;
    for(int i = 0 ; i < 10 ; i++) t_pool.enqueue(new test_tsk);

    t_pool.start();
    std::cout << "Started" << std::endl;
    t_pool.stop();

    return 0;
}
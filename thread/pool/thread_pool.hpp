#include <iostream>
#include <thread>
#include <atomic>
#include <queue>

#include <synapse/sync/event.hpp>

namespace thread
{
    class pooled_launcher { public: virtual void launch() = 0; };
    class pooled_thread
    {
        public:
            pooled_thread() : state(pool_state::await),
                              pool_thread(new std::thread(&pooled_thread::process_context, this)) { }
            ~pooled_thread() { delete pool_thread; }

            enum                 pool_state { await, running, pause_wait};
            std::atomic<uint8_t> state;

            std::thread*                 pool_thread;
            std::queue<pooled_launcher*> pool_context;
            synchronous::event<1>        pool_signal;

        private:
            void                         process_context();
    };

    template <size_t thread_count>
    class thread_pool
    {
        public:
            void launch();
            void stop  ();
            void enqueue(pooled_launcher* _ctx);
            
        private:
            pooled_thread pool_thread[thread_count];
    };
}
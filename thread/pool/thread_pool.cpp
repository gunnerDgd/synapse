#include "thread_pool.hpp"

void thread::pooled_thread::process_context()
{
    while(state != pool_state::pause_wait)
    {
        pool_signal    .await();
        if(pool_context.size() == 0) continue;

        state                 = pool_state::running;
        pooled_launcher *_ctx = pool_context.front();
                    
        _ctx                  ->launch();
        if(pool_context.size() > 0) pool_signal.alert();
    }
}

template <size_t thread_count>
void thread::thread_pool<thread_count>::launch() { for(auto pt : pool_thread) pt.pool_signal.alert(); }

template <size_t thread_count>
void thread::thread_pool<thread_count>::stop  ()
{
    for(auto pt : pool_thread)
    {
        pt.state      = pooled_thread::pool_state::pause_wait;
        pt.pool_thread->join();
    }
}

template <size_t thread_count>
void thread::thread_pool<thread_count>::enqueue(pooled_launcher* _ctx)
{
    pooled_thread* min_queued = &pool_thread[0];
    for(auto pt : pool_thread)
    if(min_queued > pt.pool_context.size()) min_queued = &pt;

    min_queued->pool_context.push(_ctx);
}
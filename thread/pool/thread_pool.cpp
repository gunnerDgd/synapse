#include "thread_pool.hpp"

thread::thread_context::thread_context(th_queue			 * _queue,
						               synchronous::fence* _lock) : ctx_queue     (_queue),
                                                                    ctx_queue_lock(_lock)
{
    ctx_th = new std::thread(&thread::thread_context::ctx_process_queue, this);
}

void					          thread::thread_context::ctx_process_queue()
{
    while(ctx_state == th_state::running)
    {
        ctx_process_event.wait();
        ctx_queue_lock  ->acquire();
        
        if(ctx_queue->size() == 0)
        {
            ctx_queue_lock->release();
            continue;
        }
        else
            ctx_process_event.alert();

        thread_task* _th_tsk = ctx_queue->front();
        ctx_queue                       ->pop  ();
        ctx_queue_lock                  ->release();

        _th_tsk          ->launch();
        _th_tsk->tsk_sig = _th_tsk;
    }
}
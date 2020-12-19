#include <synapse/sync/fence.hpp>
#include <synapse/sync/signal.hpp>

#include <queue>

namespace thread
{
	class thread_task
	{
		public:
			virtual void launch() = 0;

		private:
			synchronous::signal<thread_task*> tsk_sig;

			template <size_t N>
			friend class thread_pool;
	};
	using th_queue = std::queue<thread_task*>;

	class thread_context
	{
		public:
			thread_context(th_queue			 * _queue,
						   synchronous::fence* _lock);

		public:
			enum class th_state
			{
				running,
				stopped
			};
			th_state				 ctx_state;

		private:
			template <size_t N>
			friend class thread_pool;

			void					 ctx_process_queue();
			std::thread				*ctx_th;
			th_queue		   		*ctx_queue;

			synchronous::event  	 ctx_process_event;
			synchronous::fence 		*ctx_queue_lock;
	};

	template <size_t N>
	class thread_pool
	{
		public:
			synchronous::signal<thread_task*> enqueue(thread_task* _task);
			
			void							  start();
			void							  stop ();
			void							  wake_up()
			{
				for(int i = 0 ; i < N ; i++)
					if(th_pooled[i] != nullptr) th_pooled[i]->ctx_process_event.alert();
			}

		private:
			thread_context*    th_pooled[N] = {nullptr, };
			
			th_queue           th_queue_tsk;
			synchronous::fence th_queue_lock; 
	};
}

template <size_t N>
synchronous::signal<thread::thread_task*> thread::thread_pool<N>::enqueue(thread::thread_task* _task)
{
    th_queue_lock.acquire();
    th_queue_tsk .push   (_task);
    th_queue_lock.release();

    return        _task->tsk_sig;
}

template <size_t N>
void							  thread::thread_pool<N>::start()
{
    for(int i = 0 ; i < N ; i++)
        if(th_pooled[i] == nullptr) 
			th_pooled[i] = new thread::thread_context(&th_queue_tsk, &th_queue_lock);
    
    wake_up();
}

template <size_t N>
void							  thread::thread_pool<N>::stop ()
{
    for(int i = 0 ; i < N ; i++)
    {
        th_pooled[i]				  			->ctx_state = thread_context::th_state::stopped;
		th_pooled[i]							->ctx_process_event.alert();

        if(th_pooled[i] != nullptr) th_pooled[i]->ctx_th->join();
    }
}
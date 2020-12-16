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
            pooled_thread(size_t _id) : state      (pool_state::await),
										pool_thread(new std::thread(&pooled_thread::process_context, this)),
										pool_id(_id)	{ }
           
			~pooled_thread()							{ delete pool_thread; }

            enum                 pool_state { await, running, pause_wait };
            std::atomic<uint8_t> state;

            std::thread*                 pool_thread;
            std::queue<pooled_launcher*> pool_context;
            
			synchronous::event           pool_signal;
			size_t						 pool_id;

		public:
			using handler_finished = std::function<void(pooled_thread*)>;
			handler_finished		 on_poolFinished;

        private:
            void                     process_context()
			{
				while (state != pool_state::pause_wait)
				{
					pool_signal.wait();
					std::cout << "Pool " << pool_id << "Launched" << std::endl;

					if (pool_context.empty()) { state = pool_state::await; continue; }

					state				  = pool_state::running;
					pooled_launcher *_ctx = pool_context.front();

					_ctx	   ->launch();
					pool_context.pop();
					
					if (!pool_context.empty()) { pool_signal.alert(); }
				}
			}
    };

    template <size_t thread_count>
    class thread_pool
    {
		public:
			thread_pool() { for (int i = 0; i < thread_count; i++) pool_thread[i] = new pooled_thread(i); }

			void launch() { for (int i = 0; i < thread_count; i++) pool_thread[i]->pool_signal.alert(); }
			void stop  ()
			{
				for (int i = 0; i < thread_count; i++)
					pool_thread[i]->pool_thread->join();
			}
            
			void enqueue(pooled_launcher* _ctx)
			{
				pooled_thread* min_queued = pool_thread[0];
				for (int i = 0 ; i < thread_count ; i++)
					if (min_queued->pool_context.size() > pool_thread[i]->pool_context.size()) min_queued = pool_thread[i];

				min_queued->pool_context.push(_ctx);
				min_queued->pool_signal.alert();
			}
            
        private:
            pooled_thread* pool_thread[thread_count];
    };
}
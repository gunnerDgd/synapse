#include <synapse/sync/signal.hpp>
#include <queue>

namespace thread
{
	class pool_work
	{
	public:
		virtual void launch() = 0;
	};
	
	class pool_worker
	{
	public:
		std::queue<pool_work*> pt_queue;
		synchronous::event	   pt_queue_ready;
		bool				   pt_running = false;
		
		std::thread			  *pt_thread;
		void				   pt_process()

	};
	
	template <size_t pworker_count>
	class pool
	{
	public:
		~pool		();
		void enqueue(pool_work* pw);
		
	private:
		pool_worker p_worker[pworker_count];
	};
}

template <size_t pworker_count>
thread::pool<pworker_count>::~pool		()
{
	for(int i = 0 ; i < pworker_count ; i++){
		p_worker[i].pt_running = false;
		p_worker   .pt_thread.join();
	}
}

template <size_t pworker_count>
void thread::pool<pworker_count>::enqueue(pool_work* pw)
{
	size_t  min_queued = 0;
	for(int i = 0 ; i < pworker_count ; i++) {
		                
	}
}

void				   thread::pool_worker::pt_process()
{
	while(pt_running) {
		pt_queue_ready			.wait();
		pool_work* pw = pt_queue.front();
				
		pt_queue.pop();
		pw->launch  ();
				
		if(pt_queue.size() > 0) pt_queue.alert();
	}
}
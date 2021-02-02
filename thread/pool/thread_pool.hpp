#include <synapse/sync/signal.hpp>
#include <synapse/lockfree/queue.hpp>
#include <queue>

namespace thread
{
	class pool_work
	{
	public:
		virtual void 	   launch() = 0;
		synchronous::event pw_evt;
	};
	
	using th_queue = lockfree::cqueue<pool_work>;
	using th_event = synchronous::event;
	
	class pool_worker
	{
	public:
		pool_worker(th_queue& q, th_event& e)
			: pt_queue		(q),
			  pt_queue_ready(e) {}
		
	private:
		th_queue& pt_queue;
		th_event& pt_queue_ready;
		
		std::thread	*pt_thread;
		void		 pt_process();

	};
	
	template <size_t pworker_count>
	class pool
	{
	public:
		~pool		();
		void enqueue(pool_work* pw);
		
	private:
		pool_worker 				 p_worker[pworker_count];
		lockfree::cqueue<pool_work*> p_queue;
	};
}

template <size_t pworker_count>
thread::pool<pworker_count>::~pool		()
{

}

template <size_t pworker_count>
void thread::pool<pworker_count>::enqueue(pool_work* pw)
{
	p_queue.enqueue(pw);
}

void				   thread::pool_worker::pt_process()
{
	while(pt_running) {
		auto pw = pt_queue.dequeue();
		if  (!pw.has_value()) {
			// When Queue is Empty.
			
		}
		
		pool_work* pw_ctx = pw.value();
		
		pw_ctx			  ->launch();
		pw_ctx			  ->pw_evt.alert();
	}
}
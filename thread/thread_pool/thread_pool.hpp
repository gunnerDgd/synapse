#include <synapse/thread/work/work.hpp>
#include <synapse/lockfree/queue/queue.hpp>

#include <thread>

#define INFINITE_LOOP_START while(true) {
#define INFINITE_LOOP_END  }

namespace synapse {
namespace thread  {

	class thread_pool
	{
	public:
		using tp_queue_t = synapse::lockfree::queue<synapse::thread::work_base, 1024>;
	public:
		thread_pool		 (uint16_t tp_size);
		bool enqueue_work(synapse::thread::work_base& enq_work) { return thread_pool_worker.enqueue(enq_work); }

	private:
		std::thread *thread_pool_worker	   ;
		tp_queue_t   thread_pool_queue     ;
		void		 thread_pool_executor();
	};
}
}

synapse::thread::thread_pool::thread_pool(uint16_t tp_size)
	: thread_pool_worker(new std::thread[tp_size]{ { synapse::thread::thread_pool::thread_pool_worker, this}, }) { }

void synapse::thread::thread_pool::thread_pool_executor()
{
	INFINITE_LOOP_START
	
	synapse::thread::work_base* tp_wbase = thread_pool_queue.dequeue();
	tp_wbase							 ->execute ();
	
	INFINITE_LOOP_END
}
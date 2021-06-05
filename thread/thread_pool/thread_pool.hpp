#include <synapse/thread/work/work.hpp>
#include <synapse/lockfree/queue/queue.hpp>


#include <thread>
#include <vector>

#define INFINITE_LOOP_START while(true) {
#define INFINITE_LOOP_END  }

namespace synapse {
namespace thread  {

	class thread_pool
	{
	public:
		using tp_queue_t = synapse::lockfree::queue<synapse::thread::work_base*, 1024>;
	public:
		thread_pool		      (synapse::memory::memory& tp_memory,
							   uint16_t 			  tp_size );

		void start_thread_pool();
		void end_thread_pool  ();

	public:
		void enqueue_work(synapse::thread::work_base& enq_work);
		void execute_work();

	private:
		std::vector<std::thread*> thread_pool_worker	  ;
		uint16_t     			  thread_pool_worker_count;
		bool		 			  thread_pool_work_state  ;
		
		tp_queue_t 			   * thread_pool_queue;
		synapse::memory::memory& thread_pool_queue_memory;
	};
}
}

synapse::thread::thread_pool::thread_pool(synapse::memory::memory& tp_memory,
										  uint16_t 				   tp_size)
	: thread_pool_worker_count(tp_size)  ,
	  thread_pool_work_state  (true)     ,
	  thread_pool_queue_memory(tp_memory),
	  thread_pool_queue		  (new tp_queue_t(thread_pool_queue_memory)) { std::cout << "Complete\n"; }

void synapse::thread::thread_pool::start_thread_pool()
{
	for(uint16_t it = 0 ; it < thread_pool_worker_count ; it++)
		thread_pool_worker.push_back(new std::thread(&synapse::thread::thread_pool::execute_work, this));
}

void synapse::thread::thread_pool::end_thread_pool()
{
	thread_pool_work_state = false;
	for(auto it : thread_pool_worker) it->join();
}

void synapse::thread::thread_pool::enqueue_work(synapse::thread::work_base& enq_work)
{
	uint8_t 			   enq_retry_count = 0;
	INFINITE_LOOP_START
	if(!thread_pool_queue->enqueue(&enq_work))
	{
		enq_retry_count++;
		if(enq_retry_count == 5) {
			enq_retry_count = 0;
			sleep			(0);

			continue;
		}
	}
	else
		break;

	INFINITE_LOOP_END
	std::cout << "Enqueue Complete\n";
}

void synapse::thread::thread_pool::execute_work()
{
	uint8_t tp_retry_count = 0;
	INFINITE_LOOP_START
	
	synapse::thread::work_base* tp_wbase = *(thread_pool_queue->dequeue());
	if(!thread_pool_work_state) 		   return;

	if(!tp_wbase) {
		tp_retry_count++;
		
		if(tp_retry_count == 5) {
			tp_retry_count = 0;
			sleep			(0); // Yield Processor.
		}	continue;
	}

	tp_wbase->run ();
	INFINITE_LOOP_END
}
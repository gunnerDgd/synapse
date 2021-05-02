#include <synapse/sync/event/event.hpp>
#include <synapse/lockfree/queue/queue.hpp>

#include <thread>

namespace synapse {
namespace thread  {

	class thread_pool
	{
	public:
		thread_pool(uint16_t tp_size);

	private:
		std::thread* thread_pool_worker;
	};
}
}

synapse::thread::thread_pool::thread_pool(uint16_t tp_size)
	: 
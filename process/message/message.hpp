#include <synapse/lockfree/queue/queue.hpp>
#include <synapse/process/shared_memory/shared_memory.hpp>

namespace synapse {
namespace process {

    class message : private synapse::lockfree::queue
    {

    }
}
}
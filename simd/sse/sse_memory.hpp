#include <synapse/synapse.hpp>

namespace synapse {
namespace simd    {

    class sse_memory
    {
    public:
        
    protected:
        alignas(16) uint8_t sse_memory_context[16];
    };
}
}
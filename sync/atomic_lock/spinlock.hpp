#include <atomic>
#include <xmmintrin.h>

namespace synapse     {
namespace synchronize {
namespace atomic_lock {

    class spinlock
    {
    public:
        void acquire() { while(spinlock_flag.test_and_set(std::memory_order_acquire)) _mm_pause();  }
        void release() { spinlock_flag      .clear       (std::memory_order_release);               }

    private:
        std::atomic_flag spinlock_flag;
    };
}
}
}
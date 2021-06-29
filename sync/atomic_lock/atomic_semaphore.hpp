#pragma once
#include <synapse/sync/lock.hpp>

#include <atomic>
#include <xmmintrin.h>

namespace synapse     {
namespace synchronize {
namespace atomic_lock {

    class atomic_semaphore : public synapse::synchronize::lock
    {
    public:
        atomic_semaphore(uint32_t init_count)
            : sem_reference_count(init_count) { } 

        void acquire() override
        {
            while(sem_reference_count.load() <= 0)
                _mm_pause();
            
            sem_reference_count--;
        }

        void release() override { sem_reference_count++; }

    private:
        std::atomic<uint32_t> sem_reference_count;
    };

}
}
}
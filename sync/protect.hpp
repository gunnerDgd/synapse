#include <iostream>
#include <atomic>

#include <sched.h>

namespace synchronous
{
    class sector
    {
        public:
            void acquire()
            {
                while(ctx_protect.load() != true)
                    sched_yield();
                    
                ctx_protect = false;
            }

            void release() { ctx_protect = true; }


        private:
            std::atomic<bool> ctx_protect;
    };
}
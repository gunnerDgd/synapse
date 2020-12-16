#include <synapse/synapse.hpp>

#include <iostream>
#include <atomic>

#ifdef UNIX_MODE
#include <sched.h>
#else
#define _WINSOCKAPI_
#include <Windows.h>
#endif

namespace synchronous
{
    class fence
    {
        public:
            fence() { ctx_protect = true; }

            void acquire()
            {
				while (ctx_protect != true)
#ifdef UNIX_MODE
					sched_yield();
#else
					YieldProcessor();
#endif
                    
                ctx_protect = false;
            }

            void release() { ctx_protect = true; }


        private:
            std::atomic<bool> ctx_protect;
    };
}
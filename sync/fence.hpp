#include <synapse/synapse.hpp>

#include <iostream>
#include <thread>
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
            fence() { ctx_protect = 0; }

            void acquire()
            {
				while (ctx_protect)
                    std::this_thread::yield();

                ctx_protect = 1;
            }

            void release() { ctx_protect = 0; }

        private:
            std::atomic<int> ctx_protect;
    };
}
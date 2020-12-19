#include <iostream>
#include <synapse/synapse.hpp>

#ifdef UNIX_MODE
#include <sys/eventfd.h>
#include <unistd.h>
#else
#include <Windows.h>
#endif

namespace synchronous
{
    class event
    {
        public:
            event()
            {
#ifdef UNIX_MODE
                event_context = eventfd(0, 0);
#else
                event_context = CreateEvent(NULL, TRUE, FALSE, NULL);
#endif
            }

            void alert() 
            { 
#ifdef UNIX_MODE
                uint64_t a_ctx = 1;
                ::write   (event_context, &a_ctx, sizeof(uint64_t));
#else
                SetEvent  (event_context);
#endif
            }
            void wait ()
            {
#ifdef UNIX_MODE
                uint64_t w_ctx;
                ::read    (event_context, &w_ctx, sizeof(uint64_t));
#else
                WaitForSingleObject(event_context, INFINITE);
#endif
            }

        private:
#ifdef UNIX_MODE
            int    event_context;
#else
            HANDLE event_context;
#endif
    };
}
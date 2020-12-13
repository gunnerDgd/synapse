#include <iostream>
#include <ctime>

#ifdef UNIX_MODE
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/sem.h> 
#endif

#ifdef WIN32_MODE
#include <Windows.h>
#endif

union     semun
{
    int                  val;
    struct   semid_ds   *buf;
    unsigned short int  *array;
}         sem_ctrl;

namespace synchronous
{
    template <size_t N>
    class event
    {
        public:
            event();
            
            void await();
            void alert();

        private:
            size_t event_count = N;
            int    event_context;
    };
}
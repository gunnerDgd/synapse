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

	template          <size_t N>
	synchronous::event<N>::event()
	{
		std::srand(time());
		while (event_context != -1)
			event_context = semget(std::rand() % 1024,
				1, IPC_CREAT | IPC_PRIVATE);

		sem_ctrl.val = 0;
		semctl(event_context, 0, SETVAL, &sem_ctrl);
	}

	template          <size_t N>
	void synchronous::event<N>::await()
	{
		struct sembuf event_await = { 0, -1, SEM_UNDO };
		semop(event_context, &event_await, 1);
	}

	template          <size_t N>
	void synchronous::event<N>::alert()
	{
		struct sembuf event_await = { 0, N, SEM_UNDO };
		semop(event_context, &event_await, 1);
	}
}
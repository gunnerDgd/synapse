#include <iostream>
#include <mutex>

#include <synapse\synapse.hpp>

#define _WINSOCKAPI_
#include <Windows.h>

namespace synchronous
{
	class event
	{
	public:
		event() 
		{ 
#ifdef WIN32_MODE
			event_context = CreateEvent(NULL, TRUE, FALSE, NULL);
#else
			pthread_mutex_init(&event_context, NULL))
#endif
		}

		void wait () 
		{
#ifdef WIN32_MODE
			WaitForSingleObject(event_context, INFINITE);
			ResetEvent		   (event_context);
#else
			pthread_mutex_lock(event_context);
#endif
		}
		void alert() 
		{
#ifdef WIN32_MODE
			SetEvent		   (event_context);
#else
			pthread_mutex_unlock(event_context);
#endif
		}

	private:
#ifdef WIN32_MODE
		HANDLE			event_context;
#else
		pthread_mutex_t event_context;
#endif
	};
}
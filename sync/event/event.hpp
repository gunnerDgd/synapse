#include <iostream>
#include <mutex>

namespace synchronous
{
        template <typename T>
        class event
        {
                public:
                        event() { event_lock.lock(); }

                        void alert(T& _msg) 
                        { 
                                msg = _msg;  
                                event_lock.unlock(); 
                        }

                        T&   wait ()
                        { 
                                event_lock.lock();  
                                return msg; 
                        }

                private:
                        std::mutex event_lock;
                        T          msg;
        };
}
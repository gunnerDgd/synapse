#include <iostream>
#include <mutex>

namespace lock
{
        template <typename signal_msg>
        class signal
        {
                public:
                        signal() { signal_lock.lock(); }

                        void       alert(signal_msg& _msg) { msg = _msg;  
                                                             signal_lock.unlock(); 
                                                           }

                        signal_msg wait ()                 { 
                                                             signal_lock.lock();  
                                                             return msg; 
                                                           }

                private:
                        std::mutex signal_lock;
                        signal_msg msg;
        };
}
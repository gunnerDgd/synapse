#pragma once

#include <synapse/synapse.hpp>
#include <sys/eventfd.h>
#include <unistd.h>

namespace synapse     {
namespace synchronize {

    class event
    {
    public:
        using handle_t = int;
    public:
        event();

        bool set  ();
        bool reset();
        
        bool wait ();

        private:
            handle_t event_handle;
    };
}
}

synapse::synchronize::event::event()
    : event_handle(eventfd(0, 0)) { }

bool synapse::synchronize::event::set  () 
{ 
    uint64_t al_val = 1;
    int      al_res = ::write(event_handle, (uint8_t*)&al_val, 8);

    return  (al_res < 0) ? false : true;
}

bool synapse::synchronize::event::reset()
{
    uint64_t rs_val = 0;
    int      rs_res = ::write(event_handle, (uint8_t*)&rs_val, 8);

    return  (rs_res < 0) ? false : true;
}

bool synapse::synchronize::event::wait () 
{ 
    uint64_t w_val = 0;
    int      w_res = ::read(event_handle, (uint8_t*)&w_val, 8);

    return (w_res < 0) ? false : true;
}
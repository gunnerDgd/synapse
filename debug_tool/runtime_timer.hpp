#pragma once
#include <synapse/synapse.hpp>

#include <iostream>
#include <chrono>

namespace synapse    {
namespace debug_tool {

    using stdhtimer_t = std::chrono::time_point<std::chrono::high_resolution_clock>;
    class runtime_timer
    {
    public:
        runtime_timer(std::string msg = "[DEBUG]")
            : dbg_msg  (std::move(msg)),
              dbg_start(std::chrono::high_resolution_clock::now()) { }

        ~runtime_timer()
        {
            dbg_end = std::chrono::high_resolution_clock::now();
            std::cout << dbg_msg << " " 
                      << (float)std::chrono::duration_cast<std::chrono::nanoseconds>(dbg_end - dbg_start).count() << std::endl;
        }

    private:
        stdhtimer_t dbg_start, dbg_end;
        std::string dbg_msg           ; 
    };

}
}
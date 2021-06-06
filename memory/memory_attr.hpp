#pragma once
#include <sys/mman.h>

namespace synapse {
namespace memory  {
    
    enum lock_type     : bool { locked, unlocked };
    enum protect_type  : int
    {
        read    = PROT_READ ,
        write   = PROT_WRITE,
        execute = PROT_EXEC
    };

    int operator| (const protect_type& lhs, const protect_type& rhs) { return (int)lhs | (int)rhs; }
    int operator| (const protect_type& lhs, const int& rhs)          { return (int)lhs | rhs;      }

    enum memory_state  : int
    {
        normal          ,
        allocate_error  ,
        deallocate_error
    };

    enum io_map_state  : int
    {
        mapped,
        unmapped
    };
}
}
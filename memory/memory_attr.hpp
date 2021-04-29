#pragma once
#include <sys/mman.h>

namespace synapse
{
namespace memory
{
    enum lock_type
    {
        locked,
        unlocked
    };

    enum protect_type  : int
    {
        read    = PROT_READ ,
        write   = PROT_WRITE,
        execute = PROT_EXEC
    };

    enum memory_state
    {
        normal        ,
        allocate_error
    };
}
}
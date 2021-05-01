#pragma once

#include <iostream>
#include <string>

#include <unistd.h>
#include <sys/types.h>

namespace synapse {
namespace process {

    enum state
    {
        standby,
        running,
        error  ,
        exited
    };
}
}
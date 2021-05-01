#include <iostream>
#include <synapse/synapse.hpp>

namespace synapse {
namespace io      {

    enum class io_state
    { 
        normal       ,
        io_error     ,      // Failed in I/O Operation.
        internal_error // Failed in Initialization / System Operation.
    };
}
}
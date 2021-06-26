#pragma once

#include <iostream>
#include <X11/Xlib.h>

namespace synapse {
namespace graphic {

    using coordinate2D = std::tuple<int, int>     ;
    using coordinate3D = std::tuple<int, int, int>;
}
}
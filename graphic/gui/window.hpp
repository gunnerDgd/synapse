#include <synapse/synapse.hpp>
#include <synapse/graphic/gui/window_type.hpp>

namespace synapse {
namespace graphic {
namespace gui     {

    using window_handle_t = Window;
    class window
    {
    public:
        window(window*                        w_parent   = nullptr     ,
               synapse::graphic::coordinate2D w_position = { 50, 50 }  ,
               synapse::graphic::coordinate2D w_size     = { 640, 480 },
               
               );

        window_handle_t native_handle() { return window_handle; }
        
    private:
        window_handle_t window_handle;
    };
}
}
}
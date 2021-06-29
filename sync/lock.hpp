#include <synapse/synapse.hpp>

namespace synapse     {
namespace synchronize {

    class lock
    {
    public:
        virtual void acquire() = 0;
        virtual void release() = 0;
    };

}
}
#include <synapse/synapse.hpp>

namespace synapse {
namespace branch  {
namespace context {

    class context
    {
    public:
        uint64_t context_generic_register[16];
        uint64_t context_instruction_pointer ;
    };
}
}
}

extern "C"
{
    extern void save_context   (synapse::branch::);
    extern void restore_context();
}
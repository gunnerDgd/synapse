#include <synapse/synapse.hpp>    

namespace synapse {
namespace branch  {

    struct context
    {
        union
        {
            struct
            {
                uint64_t context_generic_register[14];
                uint64_t context_instruction_pointer ; // [rdi + 112]

                uint64_t context_stack_pointer       , // [rdi + 120]
                         context_base_pointer        ; // [rdi + 128]
            };

            uint64_t     context_register_set[17]    ;
        };
    };

}
}
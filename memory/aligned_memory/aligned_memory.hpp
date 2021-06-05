#include <synapse/memory/vmem.hpp>

namespace synapse {
namespace memory  {

    enum class memory_location { heap, page };
    
    template <synapse::memory::memory_location al_location,
              size_t                           al_parameter>
    class aligned_memory {};

    template <size_t al_page_count = 1>
    class aligned_memory<synapse::memory::memory_location::page, al_page_count> 
        : public synapse::memory::vmem
    {
    public:
        aligned_memory()
            : synapse::memory::vmem(4096 * al_page_count) { } 
    };

    template <size_t al_page_count = 1>
    class aligned_memory<synapse::memory::memory_location::heap, >

    template <size_t ap_page_count = 1>
    using aligned_page = synapse::memory::aligned_memory<synapse::memory_location::page, ap_page_count>;
}
}
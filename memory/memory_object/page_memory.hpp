#include <synapse/memory/memory_object/trait/memory_pointer_trait.hpp>

#ifdef UNIX_MODE
#include <sys/mman.h>
#include <unistd.h>
#endif

namespace memory
{
    class page_memory : public memory_pointer_trait
    {
    public:
        heap_memory           (size_t _memsize)
        : memory_pointer_trait(_memsize) { allocate(); }

    private:
        void allocate  () override
        {
            memory_pointer_context = mmap(nullptr, memory_object_size,
                                          PROT_READ | PROT_WRITE,
                                          0, -1, 0);
        }
        void deallocate() override { munmap(memory_pointer_context, memory_object_size); }
      };
}
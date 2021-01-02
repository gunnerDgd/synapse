#include <synapse/memory/memory_object/trait/memory_pointer_trait.hpp>

#ifdef UNIX_MODE
#include <sys/mman.h>
#include <unistd.h>
#endif

namespace memory
{
    class page : public pointer_trait
    {
    public:
        page           (size_t _memsize)
        : pointer_trait(_memsize) 
        {}

    public:
        void allocate  () override;
        void deallocate() override;
      };

    void page::allocate  ()
    {
        memory_pointer_context = mmap(0, memory_object_size,
                                      PROT_READ | PROT_WRITE,
                                      MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    }

    void page::deallocate() { munmap(memory_pointer_context, memory_object_size); }
}
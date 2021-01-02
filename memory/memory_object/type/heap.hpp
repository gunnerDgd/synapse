#include <iostream>
#include <synapse/memory/memory_object/trait/memory_pointer_trait.hpp>

namespace memory
{
      class heap : public pointer_trait
      {
      public:
            heap           (size_t _memsize)
            : pointer_trait(_memsize) 
            {}

      public:
            void allocate  () override;
            void deallocate() override;
      };

      void heap::allocate  () { memory_pointer_context = new uint8_t[memory_object_size]; }
      void heap::deallocate() { delete[] memory_pointer_context;  }
}
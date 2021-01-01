#include <iostream>
#include <synapse/memory/memory_object/trait/memory_pointer_trait.hpp>

namespace memory
{
      class heap_memory : public memory_pointer_trait
      {
      public:
            heap_memory (size_t _memsize)
            : memory_pointer_trait(_memsize) { allocate(); }

      public:
            void allocate  () override
            {
                  memory_pointer_context = new uint8_t[memory_object_size];
            }
            void deallocate() override 
            { 
                  delete[] memory_pointer_context; 
            }
      };
}
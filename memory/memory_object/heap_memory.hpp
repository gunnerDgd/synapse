#include <iostream>

namespace memory
{
      template <typename T>
      class heap_memory : public memory_pointer_trait<T>, public memory_scope_trait<T>
      {
      public:
            heap_memory (size_t _memsize)
            : memory_pointer_trait<T>(_memsize) {}

      private:
            void allocate()
            { 
                  memory_pointer_context = reinterpret_cast<T*>(new uint8_t[_memsize * sizeof(T)]);
            }

            void deallocate()
            {
                  delete memory_pointer_context;
            }
      }
}
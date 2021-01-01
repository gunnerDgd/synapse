#pragma once

#include <iostream>
#include <synapse/memory/memory_object/memory_object.hpp>

namespace memory
{
      class memory_pointer_trait : public memory_object
      {
      public:
            memory_pointer_trait    (size_t _size, void* _ptr = nullptr)
            : memory_pointer_context(_ptr),
              memory_object         (_size)
            {}

            const void* native_pointer() { return const_cast<const void*>(memory_pointer_context); }

      protected:
            void* memory_pointer_context;
      };
}
#pragma once
#include <iostream>
#include <functional>

#include <synapse/synapse.hpp>

namespace memory
{
      class memory_object
      {
      public:
            memory_object(size_t _memsize) : memory_object_size(_memsize)
            {}
            
      public:
            virtual void allocate  () = 0;
            virtual void deallocate() = 0;

      public:
            const size_t memory_object_size;
      };
}
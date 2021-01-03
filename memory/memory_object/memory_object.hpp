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

            size_t size() { return memory_object_size; }

      private:
            virtual void allocate  () = 0;
      public:
            virtual void deallocate() = 0;

      protected:
            size_t memory_object_size;
      };
}
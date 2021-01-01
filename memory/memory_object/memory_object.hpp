#pragma once
#include <iostream>

namespace memory
{
      template <typename T>
      class memory_object
      {
      public:
            memory_object(size_t _memsize) : memory_object_size(sizeof(T) * _memsize) 
            {}
            
            virtual allocate()   = 0;
            virtual deallocate() = 0;

            virtual T&   operator[] (size_t _offset) = 0; 
      public:
            const size_t memory_object_size;
      };
}
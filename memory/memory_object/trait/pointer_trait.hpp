#pragma once

#include <iostream>
#include <cstring>
#include <synapse/memory/memory_object/memory_object.hpp>

namespace memory
{
      class pointer_trait : public memory_object
      {
      public:
            pointer_trait           (size_t _size)
            : memory_object         (_size)
            {}

            pointer_trait           (void* _ptr, size_t _size)
            : memory_object         (_size),
              memory_pointer_context((uint8_t*)_ptr)
            {}

            pointer_trait           (pointer_trait& _copied)
            : memory_object         (_copied.memory_object_size),
              memory_pointer_context(_copied.memory_pointer_context)
            {}

            friend bool operator == (pointer_trait& lhs, pointer_trait& rhs)
            {
                  return (lhs.memory_pointer_context == rhs.memory_pointer_context)
                  &&     (lhs.memory_object_size     == rhs.memory_object_size);
            }

      public:
            friend void copy_memory(pointer_trait& _dst, pointer_trait& _src)
            {
                  size_t csize = (_dst.memory_object_size > _src.memory_object_size) ? _src.memory_object_size
                                                                                     : _dst.memory_object_size;

                  uint8_t* _pdst = _dst.memory_pointer_context,
                         * _psrc = _src.memory_pointer_context;
                               

                  while(csize--)
                        *_pdst++ = *_psrc++;
            }
            
            template <size_t N>
            void        copy_from(const char(&_src)[N])
            {
                  strncpy((char*)memory_pointer_context, _src, N);
            }

      protected:
            uint8_t*       memory_pointer_context;
      };
}
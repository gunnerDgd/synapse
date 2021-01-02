#pragma once

#include <iostream>
#include <synapse/memory/memory_object/memory_object.hpp>
#include <synapse/memory/memory_object/type/view.hpp>

namespace memory
{
      class pointer_trait : public memory_object
      {
      public:
            pointer_trait    (size_t _size)
            : memory_object  (_size)
            {}

            void*       native_pointer();
      public:
            friend void copy_from(pointer_trait& _dst, pointer_trait& _src);
            template<typename T>
            friend void copy_from(pointer_trait& _dst, view<T>& _src);
            
      protected:
            void*       memory_pointer_context;
      };

      void* pointer_trait::native_pointer() { return memory_pointer_context; }
      
      void  pointer_trait::copy_from(pointer_trait& _dst, pointer_trait& _src)
      {
            size_t copy_size = 
      }
      
      template<typename T>
      void pointer_trait::copy_from(pointer_trait& _dst, view<T>& _src)
      {

      }

      template <typename T>
      view<T> make_view(pointer_trait& _v, size_t _v_offset)
      {
            return view<T>(_v.native_pointer() + _v_offset,
                           _v.memory_object_size - _v_offset);
      }
}
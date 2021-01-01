#pragma once

#include <iostream>
#include <synapse/memory/memory_object/memory_object.hpp>

namespace memory
{
      template <typename T>
      class memory_pointer_trait : virtual public memory_object<T>
      {
      public:
            memory_pointer_trait(T* _ptr, size_t _size) 
            : memory_pointer_context(_ptr),
              memory_object<T>      (_size)
            {}

            memory_pointer_trait(size_t _size)
            : memory_object<T>(_size) 
            {}

            T& operator[] (size_t _offset) override;
            
            template <typename _Cp>
            void copy_from(memory_pointer_trait<_Cp>& _cp);

      protected:
            T* memory_pointer_context;
      };

      template <typename T>
      T& memory_pointer_trait<T>::operator[] (size_t _offset) 
      { 
            return memory_pointer_context[_offset % memory_object_size]; 
      }

      template <typename _Cp>
      void memory_pointer_trait<T>::copy_from(memory_pointer_trait<_Cp>& _cp)
      {
            size_t _cp_size = (_cp.memory_object_size > memory_object_size) ? memory_object_size
                                                                            : _cp.memory_object_size;

            memcpy(memory_pointer_context, _cp.memory_pointer_context, _cp_size);  
      }
}
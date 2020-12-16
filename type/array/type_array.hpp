#include <iostream>
#include <synapse\synapse.hpp>

namespace type
{
          template <class T>
          struct array_property
          {
                    static constexpr bool   is_array = false;
                    static constexpr size_t length    = 0;
          };

          template <class T, size_t N>
          struct array_property<T[N]>
          {
                    static constexpr bool   is_array = true;
                    static constexpr size_t length    = N;
          };

#if		  CPPVER >= 17
          template <class T>
          concept bool type_array = array_property<T>::is_array;
#endif
}
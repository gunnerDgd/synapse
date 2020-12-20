#include <iostream>

namespace memory
{
    template <typename T>
    class memory_object
    {
        public:
            memory_object();

            virtual T&               operator[]    (size_t _n) = 0;
            virtual T*               native_pointer()          { return mobj_ptr; }

            template <typename V>
            virtual memory::memory_object<V> view(size_t _len) = 0;
            
        protected:
            size_t mobj_len;
            T*     mobj_ptr;
    };
}
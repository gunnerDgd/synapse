#include <iostream>

namespace memory
{
    template <typename T>
    class view
    {
    public:
        view                (const T* _vptr, size_t _vsize)
        : view_pointer_context(_vptr),
          view_size           (_vsize) {}
        const T& operator[] (size_t _offset) { return view_pointer_context[_offset % view_size]; }

    private:
        const T* view_pointer_context;
        size_t   view_size;
    };

    template <>
    class view<char>
    {
    public:
        view                        (const char* _vptr, size_t _vsize)
        : view_pointer_context(_vptr),
          view_size           (_vsize) { }
        view                        () { }
        
        const T&        operator[]  (size_t _offset) { return view_pointer_context[_offset % view_size]; }
        friend ostream& operator << (ostream& os, view<char>& v)
        {
            os << v.view_pointer_context;
            return os;
        }

    private:
        const char* view_pointer_context;
        size_t      view_size;
    };
}
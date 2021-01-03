#include <iostream>
#include <synapse/memory/memory_object/trait/pointer_trait.hpp> 

namespace memory
{
    template <typename T>
    class access : public pointer_trait
    {
    public:
        access         (pointer_trait& _ptrait, size_t _cstart, size_t _csize)
        : pointer_trait(_ptrait)
        {
            if(_cstart + _csize > _ptrait.size())
            {
                memory_object_size     = 0;
                memory_pointer_context = nullptr;
            }
            else
            {
                memory_object_size     =  _csize;
                memory_pointer_context += _cstart;
            }
        }

        access()
        : pointer_trait(0) {}

    public:
        T&          operator[] (size_t _offset) noexcept 
        { 
            return reinterpret_cast<T*>(memory_pointer_context)[_offset % memory_object_size]; 
        }
        
        access<T>   operator+  (size_t _offset) noexcept
        {
            if(_offset >= memory_object_size)
                return access<T>();
            else
                return access<T>(*this, _offset, memory_object_size - _offset);
        }

        template <size_t N>
        void       operator=   (const T(&_src)[N])
        {
            if(N > memory_object_size) return;
            
            T* _dst = reinterpret_cast<T*>(memory_pointer_context);
            for(size_t _cpsize = 0 ; _cpsize < N ; _cpsize++)
                _dst[_cpsize]  = _src[_cpsize];
        }

        template <size_t N>
        void       operator=   (T(&_src)[N])
        {
            if(N > memory_object_size) return;
            
            T* _dst = reinterpret_cast<T*>(memory_pointer_context);
            for(size_t _cpsize = 0 ; _cpsize < N ; _cpsize++)
                _dst[_cpsize]  = _src[_cpsize];
        }

    private:
        void allocate  () override {}
        void deallocate() override {}
    };
}
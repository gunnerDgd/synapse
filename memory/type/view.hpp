#include <iostream>
#include <synapse/memory/memory_object/trait/pointer_trait.hpp> 

namespace memory
{
    template <typename T>
    class view : public pointer_trait
    {
    public:
        view           (pointer_trait& _ptrait, size_t _vstart, size_t _vsize)
        : pointer_trait(_ptrait) 
        {
            if(_vstart + _vsize > _ptrait.size())
            {
                memory_object_size     = 0;
                memory_pointer_context = nullptr;
            }
            else
            {
                memory_object_size      = _vsize;
                memory_pointer_context += _vstart;
            }
        }

        view()
        : pointer_trait(0) {}

        const T& operator[] (size_t _offset);
        view<T>  operator+  (size_t _offset);

    private:
        void allocate  () override {}
        void deallocate() override {}
    };
    
	template <typename T>
	const T& view<T>::operator[] (size_t _offset)
	{ 
		return reinterpret_cast<T*>(memory_pointer_context)[_offset % memory_object_size]; 
	}
	
	template <typename T>
	view<T>  view<T>::operator+  (size_t _offset)
	{
		if(_offset >= memory_object_size)
			return view<T>();
		else
			return view<T>(*this, _offset, memory_object_size - _offset);
	}
}
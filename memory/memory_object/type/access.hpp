#include <iostream>
#include <synapse/memory/memory_object/trait/pointer_trait.hpp> 

namespace memory
{
    template <typename T>
    class access : public pointer_trait, public access_base<T>
    {
    public:
        access         (pointer_trait& _ptrait, size_t _cstart, size_t _csize = 0)
        : pointer_trait(_ptrait)
        {
            if(_cstart + _csize > _ptrait.size())
            {
                memory_object_size     = 0;
                memory_pointer_context = nullptr;
            }
            else
            {
                memory_object_size      = (_csize == 0) ? _ptrait.size() : _csize;
                memory_pointer_context += _cstart;
            }
        }

        access()
        : pointer_trait(0) {}

	public:
		template <typename _Cp>
		friend void copy_memory(access<_Cp>& _dst,   access<_Cp>& _src);
		size_t		copy_memory(const uint8_t* _src, size_t _size)     override;

		T&          operator[] (size_t _offset)    			  		   override noexcept;
		access<T>   operator+  (size_t _offset)    			  		   override noexcept;

    private:
        void 		allocate   () 							  override {}
        void 		deallocate () 							  override {}
    };

	template <typename T>
	void		copy_memory(access<T>& _dst, access<T>& _src)
	{
		size_t csize = (_dst.memory_object_size > _src.memory_object_size) ? _src.memory_object_size
																		   : _dst.memory_object_size;

		uint8_t* _pdst = _dst.memory_pointer_context,
			   * _psrc = _src.memory_pointer_context;


		while (csize--)
			*_pdst++ = *_psrc++;
	}
	
	template <typename T>
	size_t		memory::access<T>::copy_memory(const uint8_t* _src, size_t _size)
	{
		if	 (_size > memory_object_size) return 0;
		while(_size-- > 0)
			*memory_pointer_context++ = *_src++;
		
		return _size;
	}

	template <typename T>
	T&			memory::access<T>::operator[] (size_t _offset) noexcept
	{
		return reinterpret_cast<T*>(memory_pointer_context)[_offset % memory_object_size];
	}

	template <typename T>
	access<T>   memory::access<T>::operator+  (size_t _offset) noexcept
	{
		if (_offset >= memory_object_size)
			return access<T>();
		else
			return access<T>(*this, _offset, memory_object_size - _offset);
	}
}
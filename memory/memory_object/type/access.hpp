#include <iostream>
#include <synapse/memory/memory_object/trait/pointer_trait.hpp> 

namespace memory
{
    template <typename T>
    class access : public pointer_trait
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
		friend void copy_memory(access<_Cp>& _dst, access<_Cp>& _src);

		template <size_t N>
		void        copy_from  (const T(&_src)[N])
		{
			size_t _cp_size = N;
			if    (_cp_size > memory_object_size) return;

			T*     _dst = (T*)memory_pointer_context;

			while (_cp_size--)
				*_dst++ = *_src++;
		}

		void        copy_from  (const T* _src, size_t _cp_size);

		T&          operator[] (size_t _offset)    noexcept;
		access<T>   operator+  (size_t _offset)    noexcept;

        template <size_t N>
		void        operator=  (const T(&_src)[N]) noexcept
		{
			if (N > memory_object_size) return;

			T* _dst = reinterpret_cast<T*>(memory_pointer_context);
			for (size_t _cpsize = 0; _cpsize < N; _cpsize++)
				_dst[_cpsize] = _src[_cpsize];
		}

        template <size_t N>
		void        operator=  (T(&_src)[N])	   noexcept
		{
			if (N > memory_object_size) return;

			T* _dst = reinterpret_cast<T*>(memory_pointer_context);
			for (size_t _cpsize = 0; _cpsize < N; _cpsize++)
				_dst[_cpsize] = _src[_cpsize];
		}

		bool		operator== (access<T>& _cmp)
		{
			if(_cmp.size() != memory_object_size) return false;
			else
			{
				for(size_t i = 0 ; i < memory_object_size ; i++)
					if(_cmp[i] != (*this)[i]) 	  return false;
				
				return true;
 			}
		}

    private:
        void allocate  () override {}
        void deallocate() override {}
    };
	
	template <typename T>
	void        memory::access<T>::copy_from(const T* _src, size_t _cp_size)
	{
		if (_cp_size > memory_object_size) return;
		T*  _dst = (T*)memory_pointer_context;

		while (_cp_size--)
			*_dst++ = *_src++;
	}

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
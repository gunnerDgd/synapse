#include <iostream>
#include <synapse/memory/memory_object/trait/pointer_trait.hpp> 

namespace memory
{
	template <typename T>
	using access_list  = std::vector<access<T>>;
	using access_found = std::set	<size_t>;
	
    template <typename T>
    class access : public pointer_trait
    {
    public:	
        access         (pointer_trait& _ptrait, 
						size_t 		   _cstart, 
						size_t 		   _csize = 0);

        access()
        : pointer_trait(0) {}

	public:
		template <typename _Cp>
		friend void copy_memory(access<_Cp>& _dst,   access<_Cp>& _src);
		size_t		copy_memory(const uint8_t* _src, size_t _size)     ;

		T&          operator[] (size_t _offset) noexcept;
		access<T>   operator+  (size_t _offset) noexcept;
		
	public:
		access_found 		find(memory::access<T>& delim);
		access_found		find(const uint8_t* _ctx, size_t _len);
		
		void				trim(memory::access<T>& _delim);
		void				trim(const uint8_t* _ctx, size_t _len);
		
		access_list			split(memory::access<T>& _delim);
		access_list			split(const uint8_t* _ctx, size_t _len);

    private:
        void 		allocate   ()				override {}
        void 		deallocate ()				override {}
    };

	template <typename T>
	access<T>::access         (pointer_trait& _ptrait, 
							   size_t 		   _cstart, 
							   size_t 		   _csize) : pointer_trait(_ptrait)
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
	T&			memory::access<T>::operator[] (size_t _offset)
	{
		return reinterpret_cast<T*>(memory_pointer_context)[_offset % memory_object_size];
	}

	template <typename T>
	access<T>   memory::access<T>::operator+  (size_t _offset)
	{
		if (_offset >= memory_object_size)
			return access<T>();
		else
			return access<T>(*this, _offset, memory_object_size - _offset);
	}
	
	template <typename T>
	access_found 		memory::access<T>::find(memory::access<T>& delim) { }
	
	template <typename T>
	access_found		memory::access<T>::find(const uint8_t* _ctx, size_t _len);
		
	void				trim(memory::access<T>& _delim);
	void				trim(const uint8_t* _ctx, size_t _len);
		
	access_list			split(memory::access<T>& _delim);
	access_list			split(const uint8_t* _ctx, size_t _len);
}
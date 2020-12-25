#pragma once
#include <synapse/synapse.hpp>
#include <iostream>

namespace memory
{
	template <typename V>
	class view
	{
	public:
		view(void* _view, size_t _off, size_t _len)
			: view_pointer((V*)((uint8_t*)_view + _off)),
			  view_length (_len),
			  view_offset (_off)
		{}

		view() {}

		V& operator[](size_t _off) { return view_pointer[_off % view_length]; }

	private:
		V*	   view_pointer = nullptr;
		size_t view_offset,
			   view_length;
	};

	template <>
	class view<char>
	{
	public:
		view(void* _view, size_t _off, size_t _len)
			: view_pointer((char*)((uint8_t*)_view + _off)),
			  view_length(_len),
			  view_offset(_off)
		{}

		view() {}

		char& operator[]				 (size_t _off) { return view_pointer[_off % view_length]; }
		friend std::ostream& operator << (std::ostream& _os, memory::view<char> _v)
		{
			for (int i = 0; i < _v.view_length; i++)
				_os << _v[i];

			return _os;
		}

	private:
		char*  view_pointer = nullptr;
		size_t view_offset,
			   view_length;
	};
}

using string_token = memory::view<char>;
#pragma once
#include <synapse/synapse.hpp>
#include <iostream>

namespace memory
{
	template <typename T>
	class view
	{
	public:
		view(const T* _vptr, size_t _vlen)
			: view_pointer(_vptr), view_length(_vlen) {}

		const T& operator[] (size_t _offset) { return view_pointer[_offset % view_length]; }

	private:
		size_t   view_length;
		const T* view_pointer;
	};
}

#include "string.hpp"

format::str::str(const char*& _cstr)
{
	memory::heap _str_heap(strlen(_cstr));
	_str_heap = _cstr;
}
#include "string.hpp"

format::str::str(const char*& _cstr)
{
	memory::heap 			*_str_heap = new memory::heap(strlen(_cstr));
	memory::access<uint8_t> *_str_acc  = new memory::access<uint8_t>(*_str_heap, 0);
	
	_str_acc			   ->copy_memory((const uint8_t*)_cstr, strlen(_cstr));
}
#pragma once
#include <iostream>
#include <map>

#include <synapse/memory/memory_object/trait/pointer_trait.hpp>
#include <synapse/memory/memory_object/type/access.hpp>
#include <synapse/memory/memory_object/type/page.hpp>
#include <synapse/memory/memory_object/type/heap.hpp>

namespace memory
{	
	enum class segment_location
	{
		heap,
		page
	};

	struct segment_block
	{
		memory::access<uint8_t> *block_accessor;
		memory::pointer_trait   *block_memory;
	};

	class  segment
	{
	public:
		uint8_t&		 	operator[] (size_t offset);

		void add_segment   (memory::pointer_trait* _ptr, memory::access<uint8_t>* _acc = nullptr);
		void add_segment   (size_t _size, segment_location _loc = segment_location::heap);

		void delete_segment(size_t offset);

	public:
		void copy_memory   (uint8_t* _src, size_t _size);

	private:
		std::map<size_t, segment_block*> segment_map;
	};

uint8_t& memory::segment::operator[] (size_t offset)
{
	auto lb = segment_map.lower_bound(offset);
	
	if(lb->first != offset)
		offset -= (--lb)->first;

	return (*lb->second->block_accessor)[offset];
}

void memory::segment::add_segment   (memory::pointer_trait* _ptr, memory::access<uint8_t>* _acc)
{
	struct segment_block* _segblk = new struct segment_block;
	_segblk->block_memory 		  = _ptr;
	_segblk->block_accessor		  = (!_acc) ? new memory::access<uint8_t>(*_ptr, 0) : _acc;

	size_t _new_seg_offset;
	if(segment_map.size() != 0)
		_new_seg_offset = segment_map.rbegin()->first
						+ segment_map.rbegin()->second->block_memory->size();
	else
		_new_seg_offset = 0;

	segment_map.insert(std::make_pair(_new_seg_offset, _segblk));
}

void memory::segment::add_segment   (size_t _size, segment_location _loc)
{
	memory::pointer_trait* _ptr = (_loc == segment_location::heap)
								? (reinterpret_cast<memory::pointer_trait*>(new memory::heap(_size)))
								: (reinterpret_cast<memory::pointer_trait*>(new memory::page(_size)));

	add_segment(_ptr);
}

void memory::segment::delete_segment(size_t offset)
{
	auto lb  = segment_map.lower_bound(offset);
	if  (lb != segment_map.end()) segment_map.erase(lb);
}

void memory::segment::copy_memory(uint8_t* _src, size_t _size)
{
	for(size_t i = 0 ; i < _size ; i++)
		(*this)[i] = _src[i];
}
}
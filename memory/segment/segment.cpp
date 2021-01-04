#include "segment.hpp"

uint8_t& memory::segment::operator[] (size_t offset)
{
	auto lb = segment_map.lower_bound(offset);
	offset -= lb->first;

	return (*lb->second)[offset];
}

void memory::segment::add_segment(memory::access<uint8_t>* _seg)
{
	size_t _new_seg_offset = segment_map.rbegin()->first
						   + segment_map.rbegin()->second->size();

	segment_map.insert(std::make_pair(_new_seg_offset, _seg));
}

void memory::segment::delete_segment(size_t offset)
{

}
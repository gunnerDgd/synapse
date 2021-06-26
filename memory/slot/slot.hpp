#pragma once
#include <synapse/memory/memory.hpp>
#include <synapse/memory/vmem/vmem.hpp>
#include <synapse/lockfree/stack/stack.hpp>

namespace synapse {
namespace memory  {
	
	class slot : private synapse::lockfree::stack<synapse::memory::memory>
	{
	public:
		slot(size_t slot_memory_size = 4096,
			 size_t slot_init_size   = 1024);

	public:
		synapse::lockfree::block<synapse::memory::memory>* acquire();
		void				     						   release(synapse::memory::memory& mp_rel);
	};
}
}

synapse::memory::slot::slot(size_t slot_memory_size, size_t slot_init_size)
{
	for(size_t i = 0 ; i < slot_init_size ; i++)
		this->push(synapse::memory::vmem(slot_memory_size));
}

synapse::lockfree::block<synapse::memory::memory>* synapse::memory::slot::acquire()
{
	return this->pop();
}

void synapse::memory::slot::release(synapse::memory::memory& mp_rel)
{
	this->push(mp_rel);
}
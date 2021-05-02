#pragma once
#include <synapse/memory/memory.hpp>
#include <synapse/lockfree/stack/stack.hpp>

namespace synapse {
namespace memory  {
	
	class slot : private synapse::lockfree::stack<synapse::memory::memory>
	{
	private:
		using lf_block = synapse::lockfree::block<synapse::memory::memory>;

	public:
		slot(size_t mp_count = 1024);

	public:
		synapse::memory::memory* acquire();
		void					 release(synapse::memory::memory& mp_rel);
	};
}
}

synapse::memory::slot::slot(size_t mp_count)
{
	for(int i = 0 ; i < mp_count ; i++)
		this->push(synapse::memory::vmem());
}

synapse::memory::memory* synapse::memory::slot::acquire()
{
	lf_block* ac_blk;
	do 		{ ac_blk = this->pop(); } while(!ac_blk);

	synapse::memory::memory* ac_mem = ac_blk->block_context;
	delete 					 ac_blk;

	return ac_mem;
}
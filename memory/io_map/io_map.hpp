#include <synapse/disk/file/file.hpp>

#include <synapse/memory/memory.hpp>
#include <synapse/memory/memory_attr.hpp>

namespace synapse
{
namespace memory
{
    class io_map : public synapse::memory::memory
    {
    public:
        io_map(synapse::disk::file& io_file);

        ~io_map();
        
    public:
        void* view(size_t v_size = 0,
                   size_t v_off  = 0,
                   int    v_prot = (int)protect_type::read | (int)protect_type::write,
                   void*  v_addr = nullptr);

        bool  sync();

    protected:
        synapse::disk::file& iomap_file;
    };
}
}

synapse::memory::io_map::io_map(synapse::disk::file& io_file)
    : memory    (nullptr, 0),
      iomap_file(io_file)   { }

void* synapse::memory::io_map::view(size_t v_size,
                                    size_t v_off ,
                                    int    v_prot,
                                    void*  v_addr)
{
    memory_block_size = (!v_size) ? iomap_file.size() : v_size;
    memory_address    = mmap(v_addr           ,
                             memory_block_size,
                             v_prot           ,
                             MAP_PRIVATE      ,
                             iomap_file.handle(), v_off);

    return memory_address;
}

bool  synapse::memory::io_map::sync()
{
    int     sync_res = msync(memory_address, memory_block_size, MS_SYNC);
    return (sync_res < 0) ? false : true;
}

synapse::memory::io_map::~io_map() 
{
    if(memory_lock_type == synapse::memory::lock_type::locked)
        this->unlock();
    
    munmap(memory_address, memory_block_size); 
}
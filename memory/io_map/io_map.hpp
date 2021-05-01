#include <synapse/device/file/file.hpp>

#include <synapse/memory/memory.hpp>
#include <synapse/memory/memory_attr.hpp>

namespace synapse {
namespace memory  {
    
    class io_map : public synapse::memory::memory
    {
    public:
        io_map(synapse::device::file& io_file);
        ~io_map();
        
    public:
        void* view(size_t v_size = 0,
                   size_t v_off  = 0,
                   int    v_prot = (int)protect_type::read | (int)protect_type::write,
                   void*  v_addr = nullptr);

        bool  unview();
        bool  sync  ();

    protected:
        synapse::device::file&          iomap_file;
        synapse::memory::io_map_state iomap_state;
    };
}
}

synapse::memory::io_map::io_map(synapse::device::file& io_file)
    : memory     (nullptr, 0),
      iomap_file (io_file)   ,
      iomap_state(synapse::memory::io_map_state::unmapped)   { }

void* synapse::memory::io_map::view(size_t v_size,
                                    size_t v_off ,
                                    int    v_prot,
                                    void*  v_addr)
{
    memory_block_size = (!v_size) ? iomap_file.size() : v_size;
    memory_address    = mmap(v_addr               ,
                             memory_block_size    ,
                             v_prot               ,
                             MAP_SHARED | MAP_FILE,
                             iomap_file.handle(), v_off);

    if(memory_address)
        iomap_state = synapse::memory::io_map_state::mapped;

    return memory_address;
}

bool  synapse::memory::io_map::sync()
{
    int     sync_res = msync(memory_address, memory_block_size, MS_SYNC);
    return (sync_res < 0) ? false : true;
}

bool  synapse::memory::io_map::unview()
{
    if(iomap_state == synapse::memory::io_map_state::unmapped)
        return true;

    int uv_res      = munmap(memory_address, memory_block_size);
    if (uv_res >= 0)
        iomap_state = synapse::memory::io_map_state::unmapped;

    return (uv_res < 0) ? false: true;
}

synapse::memory::io_map::~io_map() 
{
    if(memory_lock_type == synapse::memory::lock_type::locked)
        this->unlock();
    
    if(iomap_state == synapse::memory::io_map_state::mapped)
        this->unview();
}
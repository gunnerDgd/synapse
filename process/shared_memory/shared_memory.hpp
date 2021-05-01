#include <iostream>
#include <synapse/memory/memory.hpp>

#include <sys/types.h>
#include <sys/shm.h>

namespace synapse {
namespace process {

    class shared_memory : public synapse::memory::memory
    {
    public:
        shared_memory(std::string sm_key        , 
                      size_t      sm_size = 4096,
                      void*       sm_addr = nullptr);
                    
        shared_memory(std::string sm_key, 
                      void*       sm_addr = nullptr);
    
    public:
        

    private:
        key_t  shmem_key;
        size_t shmem_size;
    };

}
}

synapse::process::shared_memory::shared_memory(std::string sm_key, size_t sm_size, void* sm_addr)
    : shmem_key(std::hash<std::string>{}(sm_key)),
      memory   (nullptr, sm_size)
{
    int shmem_res = ::shmget(shmem_key, sm_size, IPC_CREAT);
    if (shmem_res < 0) return;

    memory_address = ::shmat(shmem_key, sm_addr, SHM_RND);
    if(!memory_address) return;
}

synapse::process::shared_memory::shared_memory(std::string sm_key, void)
{
}
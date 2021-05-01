#include <iostream>
#include <synapse/memory/memory.hpp>

#include <sys/types.h>
#include <sys/shm.h>

namespace synapse {
namespace process {

    enum  shared_memory_mode
    {
        create,
        open
    };

    class shared_memory : public synapse::memory::memory
    {
    public:
        using handle_t = int;
    public:
        shared_memory(std::string        sm_key,
                      shared_memory_mode sm_create = shared_memory_mode::create,
                      size_t             sm_size   = 1024,
                      void*              sm_addr   = nullptr);

        ~shared_memory();

    private:
        key_t    shmem_key;
        handle_t shmem_handle;
    };

}
}

synapse::process::shared_memory::shared_memory(std::string                          sm_key   ,
                                               synapse::process::shared_memory_mode sm_create,
                                               size_t                               sm_size  ,
                                               void*                                sm_addr)
    : shmem_key(std::hash<std::string>{}(sm_key)),
      memory   (nullptr, sm_size)
{ 
    if(sm_create == shared_memory_mode::create)
        shmem_handle   = ::shmget(shmem_key   , sm_size, IPC_CREAT | 0666);
    else
        shmem_handle   = ::shmget(shmem_key   , sm_size, 0666);
    
    if (shmem_handle < 0)  return;

    memory_address = ::shmat (shmem_handle, sm_addr, IPC_CREAT | 0666);
}

synapse::process::shared_memory::~shared_memory() { shmdt(memory_address); }
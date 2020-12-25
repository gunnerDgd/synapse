#include <iostream>
#include <list>

#include <synapse/sync/fence.hpp>
#ifdef UNIX_MODE
#include <sys/mman.h>
#else
#include <Windows.h>
#endif

namespace memory
{
    template <typename T>
    class     memory_pool
    {
        public:
            memory_pool (size_t _sz);
            ~memory_pool();

            T*		 assign (size_t   _count);
            void     release(uint8_t* _mem);

        private:
            std::list<uint8_t*>    p_memory;
            synchronous::fence     p_memory_lock;

            void*                  p_mmap_pointer;
            size_t                 p_mmap_size;
#ifdef WIN32_MODE
            HANDLE                 p_mmap_handle;
#endif        
    };

template <typename T>
memory::memory_pool<T>::memory_pool (size_t _sz) : p_mmap_size(sizeof(T) * _sz)
{
#ifdef UNIX_MODE
    p_mmap_pointer = mmap(0, _sz * sizeof(T), 
                          PROT_READ | PROT_WRITE | PROT_EXEC,
                          MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

#else
    p_mmap_handle  = CreateFileMapping(INVALID_HANDLE_VALUE,
									   NULL,
                                       PAGE_READWRITE,
                                       (p_mmap_size >> 32),
                                       (p_mmap_size & 0xFFFFFFFF), NULL);

    p_mmap_pointer = MapViewOfFile(p_mmap_handle,
                                   FILE_MAP_ALL_ACCESS,
                                   0, 0, p_mmap_size);
#endif

    for(int i = 0 ; i < _sz ; i++)
        p_memory.push_back((uint8_t*)p_mmap_pointer + i*sizeof(T));
}

template <typename T>
memory::memory_pool<T>::~memory_pool()
{
#ifdef WIN32_MODE
    UnmapViewOfFile(p_mmap_pointer);
    CloseHandle    (p_mmap_handle);
#else
    munmap(p_mmap_pointer, p_mmap_size);
#endif
}

template <typename T>
T* memory::memory_pool<T>::assign (size_t _count)
{
    //p_memory_lock.acquire();

    if(p_memory.empty()) 
    {
		return        nullptr;
    }

    uint8_t* _ret = p_memory.front();
    p_memory       .pop_front();

    //p_memory_lock.release();
    return       (T*)_ret;
}

template <typename T>
void        memory::memory_pool<T>::release(uint8_t* _mem)
{
    p_memory_lock.acquire();
    p_memory.push_back   (_mem);

    p_memory_lock.release();
}
}
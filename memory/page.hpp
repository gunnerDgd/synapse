#pragma once
#include <synapse/memory/memory_object.hpp>
#ifdef WIN32_MODE
#include <Windows.h>
#else
#include <sys/mman.h>
#include <unistd.h>
#endif

#define  X64_PAGE_SIZE 4096*1024

namespace memory
{
	template <typename T>
	class page_object : public memory::memory_pointer_trait<T>
	{
	public:
		page_object(uint64_t _size, void* _base = nullptr)
			: memory::memory_pointer_trait<T> (nullptr, _size, _size * sizeof(T)),
			  page_base_address			      (_base)
		{
#ifdef WIN32_MODE
			page_map_handle = CreateFileMapping(INVALID_HANDLE_VALUE,
												NULL,
												PAGE_READWRITE,
											   (memory_size >> 32),
											   (memory_size & 0xFFFFFFFF), NULL);

			memory_pointer = MapViewOfFileEx(page_map_handle,
										     FILE_MAP_ALL_ACCESS,
											 0, 0, memory_size, _base);
#else
			memory_pointer = mmap(0, _size * sizeof(T),
								  PROT_READ | PROT_WRITE | PROT_EXEC,
								  MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
#endif
		}

		page_object(HANDLE _hnd_map, size_t _off, size_t _size)
		{
			page_map_handle = _hnd_map;
			memory_pointer = MapViewOfFileEx(page_map_handle,
											 FILE_MAP_ALL_ACCESS,
											 _off >> 32,
											 _off & 0xFFFFFFFF, _size, NULL);
		}
		
		~page_object()
		{
			UnmapViewOfFile(memory_pointer);
			CloseHandle	   (page_map_handle);
		}

		T& operator[](size_t _off)
		{
			return reinterpret_cast<T*>(memory_pointer)[_off % memory_array_size];
		}

	private:
		void*  page_base_address;
#ifdef WIN32_MODE
		HANDLE page_map_handle;
#endif
	};
}
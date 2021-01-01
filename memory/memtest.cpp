#include <synapse/memory/memory_object/heap_memory.hpp>

#include <iostream>
#include <chrono>

int main()
{
    int* test[1024];
    auto start = std::chrono::high_resolution_clock::now();

    for(int i = 0 ; i < 1024 ; i++) test[i] = new int;

    auto end   = std::chrono::high_resolution_clock::now();

    std::cout << (end - start).count() << std::endl;

    start = std::chrono::high_resolution_clock::now();

    for(int i = 0 ; i < 1024 ; i++) 
    {
        memory::heap_memory tst_heap(4);
        test[i] = (int*)tst_heap.native_pointer();
    }
    
    end   = std::chrono::high_resolution_clock::now();

    std::cout << (end - start).count() << std::endl;
}
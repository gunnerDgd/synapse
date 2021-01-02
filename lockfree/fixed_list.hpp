#include <iostream>
#include <atomic>

namespace lockfree
{
    template <typename T>
    class fixed_list_block
    {
    public:
        T                    block_context;
        fixed_list_block<T>* block_next = nullptr;
    }
    template <typename T, size_t N>
    class fixed_list
    {
    public:
        fixed_list();

    public:
        fixed_list_block<T>* operator[] (size_t offset);
        void

    public:
        void front_to_back();
        void back_to_front();
        
    private:
        static constexpr                  list_length = N;
    private:
        std::atomic<fixed_list_block<T>*> list_entry,
                                          list_end;

        fixed_list_block<Ts>             * list_base;
    }

    template <typename T, size_t N>
    fixed_list<T, N>::fixed_list()
    {
        list_base = new fixed_list_block<T>[N];
        
        list_entry.store(list_base);
        list_end  .store(&(list_base[N - 1]));
    }

    template <typename T, size_t N>
    fixed_list_block<T>* fixed_list<T, N>::operator[] (size_t offset)  { return list_base[offset % list_length]; }

}
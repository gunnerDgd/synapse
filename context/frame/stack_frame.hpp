#pragma once
#include <iostream>

#define IN_FUNC __attribute__((always_inline))

namespace frame
{
    class stack
    {
    public:
        stack();
        stack(size_t stack_size);

        template <typename T>
        stack(T& stack_init, size_t stack_size);

    private:
        void* stack_base = nullptr,
              stack_top  = nullptr;
    };
}

frame::stack::stack();
frame::stack::stack(size_t stack_size);

template <typename T>
frame::stack::stack(T& stack_init, size_t stack_size);
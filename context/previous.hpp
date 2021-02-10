#include <iostream>
#define IN_FUNC __attribute__((always_inline))

namespace frame
{
    class previous
    {
    public:
        enum index : uint16_t
        {
            previous_base_pointer = 0,
            previous_instruction  = 1
        };

    public:
        inline            IN_FUNC previous   ();
        inline uint64_t*& IN_FUNC operator[] (index it) { return prev_set[it]; }

    private:
        uint64_t* prev_set[2];
    };
}

inline IN_FUNC frame::previous::previous()
{
    asm volatile
    (
        "leaq 0x00(%%rbp), %0\n\t"
        "leaq 0x08(%%rbp), %1"
    :   "=g"(prev_set[0]),
        "=g"(prev_set[1])
    );
}
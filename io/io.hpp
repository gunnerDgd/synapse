#include <synapse/io/io_flag.hpp>
#include <synapse/memory/memory.hpp>

namespace synapse {
namespace io      {
    
    class io
    {
    public:
        io()
            : io_state_flag(io_state::normal) { }

    public:
        virtual size_t read (uint8_t* r_ctx, size_t r_size)  = 0;
        virtual size_t read (synapse::memory::memory& r_mem) = 0;

        virtual size_t write(uint8_t* w_ctx, size_t w_size)  = 0;
        virtual size_t write(synapse::memory::memory& w_mem) = 0;

		template <class T, size_t N>
		stream& operator << (T(&w_ctx)[N]) { write((uint8_t*)w_ctx, sizeof(T) * N); return *this; }

        template <class T>
        stream& operator << (T&& w_ctx)    { write((uint8_t*)&w_ctx, sizeof(T));    return *this; }

        template <class T>
        stream& operator << (T& w_ctx)     { write((uint8_t*)&w_ctx, sizeof(T));    return *this; }


		template <class T, size_t N>
		stream& operator >> (T(&r_ctx)[N]) { read ((uint8_t*)r_ctx, sizeof(T) * N); return *this; }

        template <class T>
        stream& operator >> (T& r_ctx)     { read ((uint8_t*)&r_ctx, sizeof(T));    return *this; }

        template <class T>
        stream& operator >> (T&& r_ctx)    { read ((uint8_t*)&r_ctx, sizeof(T));    return *this; }


    public:
        io_state current_state()                   { return io_state_flag ; }
        void     set_state    (io::io_state state) { io_state_flag = state; }

    protected:
        io_state io_state_flag;
    };
}
}

size_t synapse::io::io::read (synapse::memory::memory& r_mem)
{
    if(r_mem.current_state() != synapse::memory::memory_state::normal)
        return 0; // Check Memory State.

    return this->read(r_mem.memory_pointer(), r_mem.memory_size());
}

size_t synapse::io::io::write(synapse::memory::memory& w_mem)
{
    if(w_mem.current_state() != synapse::memory::memory_state::normal)
        return 0; // Check Memory State.

    return this->write(w_mem.memory_pointer(), w_mem.memory_size());
}
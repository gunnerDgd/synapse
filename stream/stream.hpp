#include <iostream>
#include <synapse/synapse.hpp>

namespace synapse
{
namespace stream
{
    enum class stream_state
    { 
        normal, 
        io_error,      // Failed in I/O Operation.
        internal_error // Failed in Initialization / System Operation.
    };
    
    class stream
    {
    public:
        stream()
            : stream_state_flag(stream_state::normal) { }

    public:
        virtual size_t read (uint8_t* r_ctx, size_t r_size) = 0;
        virtual size_t write(uint8_t* w_ctx, size_t w_size) = 0;

		template <class T, size_t N>
		stream& operator << (T(&w_ctx)[N]) { write((uint8_t*)w_ctx, sizeof(T) * N); return *this; }

		template <class T, size_t N>
		stream& operator >> (T(&r_ctx)[N]) { read ((uint8_t*)r_ctx, sizeof(T) * N); return *this; }

        template <class T>
        stream& operator << (T& w_ctx)     { write((uint8_t*)&w_ctx, sizeof(T));    return *this; }

        template <class T>
        stream& operator >> (T& r_ctx)     { read ((uint8_t*)&r_ctx, sizeof(T));    return *this; }

        template <class T>
        stream& operator << (T&& w_ctx)    { write((uint8_t*)&w_ctx, sizeof(T));    return *this; }

        template <class T>
        stream& operator >> (T&& r_ctx)    { read ((uint8_t*)&r_ctx, sizeof(T));    return *this; }


    public:
        stream_state state() { return stream_state_flag; }

    protected:
        stream_state stream_state_flag;
    };
}
}
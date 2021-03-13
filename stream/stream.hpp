#include <iostream>
#include <synapse/synapse.hpp>

namespace synapse
{
namespace stream
{
    class stream
    {
    public:
        virtual size_t read (uint8_t* r_ctx, size_t r_size) = 0;
        virtual size_t write(uint8_t* w_ctx, size_t w_size) = 0;

		template <class T, size_t N>
		stream& operator << (T(&w_ctx)[N]);

		template <class T, size_t N>
		stream& operator >> (T(&r_ctx)[N]);

        template <class T>
        stream& operator << (T& w_ctx);

        template <class T>
        stream& operator >> (T& r_ctx);
    };
}
} using namespace synapse;

template <class T, size_t N>
stream& stream::stream::operator << (T(&w_ctx)[N])
{
	write((uint8_t*)w_ctx, N);
	return *this;
}

template <class T, size_t N>
stream& stream::stream::operator >> (T(&r_ctx)[N])
{
	read((uint8_t*)r_ctx, N);
	return *this;
}

template <class T>
stream& stream::stream::operator << (T& w_ctx)
{
    write((uint8_t*)&w_ctx, sizeof(T));
    return *this;
}

template <class T>
stream& stream::stream::operator >> (T& r_ctx)
{
    read((uint8_t*)&r_ctx, sizeof(T));
    return *this;
}
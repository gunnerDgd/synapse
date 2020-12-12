#include "stream.hpp"

template <class T> requires type::type_array<T>
stream& stream::stream::operator << (T& w_ctx)
{
    write((uint8_t*)w_ctx, type::array_property<T>::length);
            return *this;
}

template <class T>
stream& stream::stream::operator << (T& w_ctx)
{
    write((uint8_t*)&w_ctx, sizeof(T));
    return *this;
}

template <class T> requires type::type_array<T>
stream& stream::stream::operator >> (T& r_ctx)
{
    read((uint8_t*)r_ctx, type::array_property<T>::length);
    return *this;
}

template <class T>
stream& stream::stream::operator >> (T& r_ctx)
{
    read((uint8_t*)&r_ctx, sizeof(T));
    return *this;
}
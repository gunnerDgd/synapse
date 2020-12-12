#include <iostream>
#include <functional>

#include <synapse/type/array/type_array.hpp>
#include <synapse/thread/lock/protect/protect.hpp>

namespace stream
{
    class stream
    {
        public:
            virtual size_t read (uint8_t* r_ctx, size_t r_size) = 0;
            virtual size_t write(uint8_t* w_ctx, size_t w_size) = 0;

        template <class T> requires type::type_array<T>
        stream& operator << (T& w_ctx)
        {
            write((uint8_t*)w_ctx, type::array_property<T>::length);
            return *this;
        }

        template <class T>
        stream& operator << (T& w_ctx)
        {
            write((uint8_t*)&w_ctx, sizeof(T));
            return *this;
        }

        template <class T> requires type::type_array<T>
        stream& operator >> (T& r_ctx)
        {
            read((uint8_t*)r_ctx, type::array_property<T>::length);
            return *this;
        }

        template <class T>
        stream& operator >> (T& r_ctx)
        {
            read((uint8_t*)&r_ctx, sizeof(T));
            return *this;
        }

        void               lock_stream()   { stream_locked = true; }
        void               unlock_stream() { stream_locked = false; }

        private:
            bool               stream_locked;
            lock::protect stream_lock;
    };
}
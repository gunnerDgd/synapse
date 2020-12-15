#include <iostream>
#include <functional>

#include <synapse/type/array/type_array.hpp>
#include <synapse/sync/protect.hpp>

namespace stream
{
    class stream
    {
        public:
            virtual size_t read (uint8_t* r_ctx, size_t r_size) = 0;
            virtual size_t write(uint8_t* w_ctx, size_t w_size) = 0;

            enum class stream_mode
            {
                sync,
                async
            };
            void           mode (stream_mode s_mode) { sync_mode = s_mode; }

        stream& operator << (std::string& w_ctx) { write((uint8_t*)w_ctx.c_str(), w_ctx.length()); }

        template <class T> requires type::type_array<T>
        stream& operator << (T& w_ctx);

        template <class T>
        stream& operator << (T& w_ctx);

        template <class T> requires type::type_array<T>
        stream& operator >> (T& r_ctx);

        template <class T>
        stream& operator >> (T& r_ctx);

        protected:
            stream_mode         sync_mode;

            synchronous::sector read_lock,
                                write_lock;
    };
    
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
}
#include <iostream>
#include <functional>

#include <synapse/type/array/type_array.hpp>
#include <synapse/sync/protect/protect.hpp>

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

        template <class T> requires type::type_array<T>
        stream& operator << (T& w_ctx);

        template <class T>
        stream& operator << (T& w_ctx);

        template <class T> requires type::type_array<T>
        stream& operator >> (T& r_ctx);

        template <class T>
        stream& operator >> (T& r_ctx);

        private:
            stream_mode         sync_mode;

            synchronous::sector read_lock,
                                write_lock;
    };
}
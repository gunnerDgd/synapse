#include <iostream>
#include <functional>

#include <synapse/synapse.hpp>
#include <synapse/type/array/type_array.hpp>
#include <synapse/sync/fence.hpp>

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

			stream& operator << (std::string& w_ctx);
            stream& operator >> (std::string& r_ctx);

#if		CPPVER >= 17
        template <class T> requires type::type_array<T>
        stream& operator << (T& w_ctx);

		template <class T> requires type::type_array<T>
		stream& operator >> (T& r_ctx);

#else

		template <class T, size_t N>
		stream& operator << (T(&w_ctx)[N]);

		template <class T, size_t N>
		stream& operator >> (T(&r_ctx)[N]);
#endif

        template <class T>
        stream& operator << (T& w_ctx);

        template <class T>
        stream& operator >> (T& r_ctx);

        protected:
            stream_mode        sync_mode;

            synchronous::fence read_lock,
                               write_lock;
    };
    
#if CPPVER >= 17
    template <class T> requires type::type_array<T>
    stream& stream::stream::operator << (T& w_ctx)
    {
        write((uint8_t*)w_ctx, type::array_property<T>::length);
        return *this;
    }

	template <class T> requires type::type_array<T>
	stream& stream::stream::operator >> (T& r_ctx)
	{
		read((uint8_t*)r_ctx, type::array_property<T>::length);
		return *this;
	}

#else

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

#endif

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

    stream& stream::stream::operator << (std::string& w_ctx)
    { 
        write((uint8_t*)w_ctx.c_str(), w_ctx.length()); 
        return *this; 
    }

    stream& stream::stream::operator >> (std::string& r_ctx)
    { 
        char   r_buf[4096] = { 0x00, };
        size_t r_size      = read((uint8_t*)r_buf, 4096);

        r_ctx  .assign((const char*)r_buf, r_size);
        return *this;
    }
}
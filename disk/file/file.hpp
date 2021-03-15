#include <iostream>
#include <synapse/stream/stream.hpp>
#include <synapse/disk/file/file_flag.hpp>

namespace synapse
{
    using synapse::stream::io_mode;
namespace disk
{
    class file : public stream::stream
    {
    public:
        file (std::string n, disk::access_mode a, disk::open_mode o);
		file () : stream(io_mode::block) {}
			
        ~file();

		bool open (std::string n, access_mode m, open_mode o);
		void close();

    public:
        size_t read  (uint8_t* r_ctx, size_t r_size) override;
        size_t write (uint8_t* w_ctx, size_t w_size) override;

    public:
        size_t size  ();
		void   offset(size_t m_ptr);

    private:
        std::string file_path;
        size_t      file_size;
        handle_t    file_handle;
        
#ifdef WIN32_MODE
        OVERLAPPED  file_overlapped;
#endif
    };
}
} using namespace synapse;

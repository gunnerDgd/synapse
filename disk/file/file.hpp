#include <iostream>
#include <synapse/stream/stream.hpp>
#include <synapse/disk/file/file_flag.hpp>

namespace synapse
{
namespace disk
{
#ifdef UNIX_MODE
	using handle_t = int;
#else
	using handle_t = HANDLE;
#endif
    class file : public stream::stream
    {
        public:
            file (std::string n, file::access_mode a, io_mode i);
			file () {}
			
            ~file();

			bool open (std::string n, file::access_mode m, io_mode i);
			void close();

            size_t read   (uint8_t* r_ctx, size_t r_size) override;
            size_t write  (uint8_t* w_ctx, size_t w_size) override;
			
			void   offset (size_t m_ptr);

        private:
            std::string file_path;
            size_t      file_size;
			handle_t    file_handle;
    };
}
} using namespace synapse;

disk::file::file(std::string n, access_mode m, io_mode i)
	: stream   (i),
      file_path(_name) { this->open(n, m, i); }

disk::file::~file()    { this->close(); }

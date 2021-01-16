#include <iostream>
#include <synapse/stream/stream.hpp>
#include <synapse/disk/disk_object.hpp>
#include <synapse/disk/file/file_flag.hpp>
//#include <synapse/disk/directory/directory.hpp>

#ifdef UNIX_MODE
#include <sys/types.h> 
#include <sys/stat.h>
#include <sys/mman.h>

#include <fcntl.h>
#include <unistd.h>

#include <errno.h>

extern int errno;
#else
#include <Windows.h>

#endif

namespace disk
{
    class file : public stream::stream
    {
        public:
            file (std::string _name, file::access_mode _mode);
            ~file();

			void close()
			{
#ifdef UNIX_MODE
				close(f_handle);
#else
				CloseHandle(f_handle);
#endif
			}

            size_t read   (uint8_t* r_ctx, size_t r_size) override;
            size_t write  (uint8_t* w_ctx, size_t w_size) override;
			
			void   offset (size_t m_ptr);

        private:
            std::string f_path;
            size_t      f_size;

#ifdef UNIX_MODE
            int         f_handle;
#else
            HANDLE      f_handle;
#endif
    };
}
#include <iostream>
#include <synapse/include/stream.hpp>
#include <synapse/disk/disk_object.hpp>
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
    class file : public stream::stream, public disk::disk_object
    {
        public:
            enum state
            {
                mapped,
                general
            };

#ifdef UNIX_MODE
            enum access_mode
            {
                read_only    = O_RDONLY,
                write_only   = O_WRONLY,
                all          = O_RDWR
            };
            using error_code = int;
#else
            enum access_mode
            {
                read_only    = GERNERIC_READ,
                write_only   = GENERIC_WRITE,
                all          = GENERIC_ALL
            };
            using error_code = DWORD;
#endif
        public:
            file (std::string _name, file::access_mode _mode);
            ~file();

            size_t read (uint8_t* r_ctx, size_t r_size) override;
            size_t write(uint8_t* w_ctx, size_t w_size) override;

            using error_handler = std::function<void(disk::file*, error_code)>;
            using read_handler  = std::function<void(disk::file*, uint8_t*, size_t)>;
            using write_handler = std::function<void(disk::file*, size_t)>;

            error_handler on_error;
            read_handler  on_read;
            write_handler on_write;

        public:
            void* map  ();
            void  unmap();

        private:
            void*       f_mmap_pointer;
#ifdef WIN32_MODE
            HANDLE      f_mmap_handle;
#endif


        private:
            std::string f_path;
            size_t      f_size;

            size_t      f_pointer;
            state       f_state = file::state::general;

#ifdef UNIX_MODE
            int         f_handle;
#else
            HANDLE      f_handle;
#endif
    };
}
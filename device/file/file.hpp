#include <synapse/io/io.hpp>
#include <synapse/device/file/file_flag.hpp>

namespace synapse {
namespace device  {
    
    class file : public synapse::io::io
    {
    public:
#ifdef ENV_UNIX
        using handle_t = int;
#else
        using handle_t = HANDLE;
#endif

        file (std::string f_name);			
        ~file();

		bool open (device::access_mode f_ac = device::access_mode::all,
                   device::open_mode   f_op = device::open_mode::create);
                
		bool close();

    public:
        size_t   read  (uint8_t* r_ctx, size_t r_size) override;
        size_t   write (uint8_t* w_ctx, size_t w_size) override;

    public:
        size_t   size  ();
        bool     resize(size_t f_size);
		size_t   offset(size_t f_ptr) ;

        handle_t handle() { return file_handle; }

    protected:
        std::string file_name;
        handle_t    file_handle;
    };
}
}

synapse::device::file::file(std::string f_name)
	: io       ( ),
      file_name(std::move(f_name)) { }

synapse::device::file::~file() { this->close(); }

bool synapse::device::file::open (device::access_mode f_ac, 
                                  device::open_mode   f_op)
{
#ifdef ENV_UNIX
    if(f_op == device::open_mode::create)
        file_handle = ::open(file_name.c_str(), 
                            (int)f_ac | (int)f_op,
                             S_IRWXU | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    else
        file_handle = ::open(file_name.c_str(), (int)f_ac);

    if (file_handle < 0)
    {
        io_state_flag = synapse::io::io_state::internal_error;
        return false;
    }
    else
        return true ;
#endif
}

bool   synapse::device::file::close()
{
#ifdef ENV_UNIX
	int cl_res = ::close(file_handle);
    if (cl_res < 0)
    {
        io_state_flag = synapse::io::io_state::internal_error;
        return false;
    }
    else
        return true ;
#endif
}

size_t synapse::device::file::read(uint8_t* c, size_t s)
{
#ifdef ENV_UNIX
    ssize_t r_res = ::read(file_handle, (void*)c, s);
    if     (r_res < 0)
    {
        io_state_flag = synapse::io::io_state::io_error;
        return 0;
    }
    else
        return r_res;
#endif
}

size_t synapse::device::file::write(uint8_t* c, size_t s)
{
#ifdef ENV_UNIX
    ssize_t w_res = ::write(file_handle, (void*)c, s);
    if     (w_res < 0)
    {
        io_state_flag = synapse::io::io_state::io_error;
        return 0;
    }
    else
        return w_res;
#endif
}

size_t synapse::device::file::offset(size_t f_ptr)
{
#ifdef ENV_UNIX
	off_t off_res = lseek(file_handle, f_ptr, SEEK_SET);
    if   (off_res == -1)
    {
        io_state_flag = synapse::io::io_state::internal_error;
        return 0;
    }
    else
        return off_res;
#endif
}

size_t synapse::device::file::size  ()
{
#ifdef ENV_UNIX	
    struct stat st_ctx;
    int         st_res = fstat(file_handle, &st_ctx);

    if(st_res < 0)
    {
        io_state_flag = synapse::io::io_state::internal_error;
        return 0;
    }
    else
	    return st_ctx.st_size;
#endif
}

bool   synapse::device::file::resize(size_t f_size)
{
    int rs_res = ftruncate(file_handle, f_size);
    if (rs_res < 0)
        io_state_flag = synapse::io::io_state::internal_error;

    return (rs_res >= 0) ? true : false;
}
#include "file.hpp"

disk::file::file        (std::string _name, file::mode _mode) : f_path(_name), disk::disk_object(disk::type::file)
{
#ifdef UNIX_MODE
    f_handle = open             (_name.c_str(), _mode);
    if(f_handle < 0 && on_error) on_error(this, errno);

#else
    f_handle = CreateFile(_name.c_str(), _mode,
                          0, NULL, CREATE_NEW, 0, NULL);
    
    if(f_handle < 0 && on_error) on_error(this, GetLastError());
#endif
}

size_t disk::file::read (uint8_t* r_ctx, size_t r_size)
{
#ifdef UNIX_MODE

    if(sync_mode == stream::stream_mode::sync) read_lock.acquire(); 
    size_t _sz    = read(f_handle, (void*)r_ctx, r_size);

    if(sync_mode == stream::stream_mode::sync) read_lock.release();
    if    (_sz   <= 0)
    {
        if(on_error) { on_error(this, errno); return _sz; }
        else         { return _sz; }
    }

    if    (on_read)  { on_read (this, _sz); return 0; }
    else                                    return _sz;

#else

    if(sync_mode == stream::stream_mode::sync) read_lock.acquire(); 
    size_t _sz;
    bool   r_success = ReadFile(f_handle, (void*)r_ctx, r_size, &_sz, NULL);

    if(sync_mode == stream::stream_mode::sync) read_lock.release();
    if    (!r_success)
    {
        if(on_error) { on_error(this, GetLastError()); return _sz; }
        else         { return _sz; }
    }

    if    (on_read)  { on_read (this, _sz); return 0; }
    else                                    return _sz;

#endif
}

size_t disk::file::write(uint8_t* w_ctx, size_t w_size)
{
#ifdef UNIX_MODE

    if(sync_mode == stream::stream_mode::sync) write_lock.acquire(); 
    size_t _sz    = write(f_handle, (void*)w_ctx, w_size);

    if(sync_mode == stream::stream_mode::sync) write_lock.release();
    if    (_sz   <= 0)
    {
        if(on_error) { on_error(this, errno); return _sz; }
        else         { return _sz; }
    }

        if    (on_write) { on_write(this, _sz); return 0; }
        else                                    return _sz;

#else

    if(sync_mode == stream::stream_mode::sync) write_lock.acquire(); 
    size_t _sz;
    bool   r_success = WriteFile(f_handle, (void*)w_ctx, w_size, &_sz, NULL);

    if(sync_mode == stream::stream_mode::sync) write_lock.release();
    if    (!r_success)
    {
        if(on_error) { on_error(this, GetLastError()); return _sz; }
        else         { return _sz; }
    }

    if    (on_write) { on_write(this, _sz); return 0; }
    else                                    return _sz;

#endif
}
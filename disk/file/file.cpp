#include "file.hpp"

disk::file::file        (std::string _name, file::access_mode _mode) : f_path(_name)
{
#ifdef UNIX_MODE
    f_handle = open             (_name.c_str(), O_CREAT, _mode);
	
    struct stat _fstat;
    fstat       (f_handle, &_fstat);

    f_size = _fstat.st_size;

#else
    f_handle = CreateFile(_name.c_str(), _mode,
                          0, NULL, OPEN_ALWAYS, 0, NULL);

    DWORD _hsize = 0;
    
    f_size       = GetFileSize(f_handle, &_hsize);
    f_size       = ((size_t)_hsize << 32) | f_size;
#endif
}

disk::file::~file()
{
	close();
}

size_t disk::file::read (uint8_t* r_ctx, size_t r_size)
{
#ifdef UNIX_MODE
    return ::read(f_handle, (void*)r_ctx, r_size);

#else
    size_t _sz;
    bool   r_success = ReadFile(f_handle, (void*)r_ctx, r_size, (LPDWORD)&_sz, NULL);

	return _sz;
#endif
}

size_t disk::file::write(uint8_t* w_ctx, size_t w_size)
{
#ifdef UNIX_MODE

    return ::write(f_handle, (void*)w_ctx, w_size);

#else
    size_t _sz;
    bool   r_success = WriteFile(f_handle, (void*)w_ctx, w_size, (LPDWORD)&_sz, NULL);

	return _sz;
#endif
}

void   disk::file::offset(size_t m_ptr)
{
#ifdef WIN32_MODE
	LONG _hptr   = m_ptr >> 32;
	SetFilePointer(f_handle,
				   m_ptr & 0xFFFFFFFF, &_hptr,
				   FILE_BEGIN);
#else
	lseek		  (f_handle, m_ptr, SEEK_SET);
#endif
}
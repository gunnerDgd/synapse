#include <synapse/disk/file/file.hpp>
using namespace synapse;

disk::file::file(std::string n, access_mode m, open_mode o)
	: stream   (io_mode::block),
      file_path(n)  		   { this->open(n, m, o); }

disk::file::~file() 		   { this->close(); }

bool   disk::file::open (std::string n, disk::access_mode m, disk::open_mode o)
{
#ifdef UNIX_MODE
	if(o == open_mode::create)
		file_handle  = ::open(n.c_str(), m | O_CREAT | O_EXCL, S_IRWXU);
	else
		file_handle  = ::open(n.c_str(), m | O_CREAT		 , S_IRWXU);
	
	if(file_handle <= 0) return false;
	else {
		this->size();
		return  true;
	}
#else
	file_handle     = CreateFile(n.c_str(), m, 0, NULL, o, NULL, NULL);
	
	if(file_handle == INVALID_HANDLE_VALUE) return false;
	else {
		this->size();
		return  true;
	}
#endif
}

void   disk::file::close()
{
#ifdef UNIX_MODE
	::close	   (file_handle);
#else
	CloseHandle(file_handle);
#endif
}

size_t disk::file::read (uint8_t* c, size_t s)
{
#ifdef UNIX_MODE
    return ::read(file_handle, (void*)c, s);
#else
    size_t r_size;
	bool   r_success;

	switch(stream_mode)
	{
	case io_mode::block:
    	r_success = ReadFile(file_handle, (void*)c, s, (LPDWORD)&r_size, NULL);
		break;
	case io_mode::non_block:
		r_success = ReadFile(file_handle, (void*)c, s, (LPDWORD)&r_size, &file_overlapped);
		break;
	}

	return (!r_success) ? 0 : r_size;
#endif
}

size_t disk::file::write(uint8_t* c, size_t s)
{
#ifdef UNIX_MODE
    return ::write(file_handle, (void*)c, s);
#else
    size_t w_size;
	bool   w_success;

	switch(stream_mode)
	{
	case io_mode::block:
    	w_flag = WriteFile(file_handle, (void*)c, s, (LPDWORD)&w_size, NULL);
		break;

	case io_mode::non_block:
		w_flag = WriteFile(file_handle, (void*)c, s, (LPDWORD)&w_size, &file_overlapped);
		break;
	}

	return (!w_flag) ? 0 : w_size;
#endif
}


void   disk::file::offset(size_t m_ptr)
{
#ifdef WIN32_MODE
	LONG _hptr   = m_ptr >> 32;
	SetFilePointer(file_handle,
				   m_ptr & 0xFFFFFFFF, &_hptr,
				   FILE_BEGIN);
#else
	lseek		  (file_handle, m_ptr, SEEK_SET);
#endif
}

size_t disk::file::size  ()
{
#ifdef UNIX_MODE	
    struct stat _fstat;
    fstat       (file_handle, &_fstat);

    file_size = _fstat.st_size;
	return      file_size;

#else
    DWORD size_high = 0;   
    file_size       = GetFileSize(file_handle, &size_high);
    file_size       = ((size_t)size_high << 32) | f_size;
	
	return file_size;
#endif
}
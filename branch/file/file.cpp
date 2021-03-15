#include <synapse/branch/file/file.hpp>
using namespace synapse;

branch_io::file::file(std::string n, disk::open_mode o, disk::access_mode a)
{
    this->open(n, o, a);
}
    
void branch_io::file::open (std::string n, disk::open_mode o, disk::access_mode a)
{
#ifdef UNIX_MODE
    file_handle = open(n.c_str(),
                      (o == disk::open_mode::create) ? (o | a | O_CREAT | O_EXCL | O_NONBLOCK)
                                                     : (o | a | O_CREAT | O_NONBLOCK),
                      S_IRWXU);
    
    if(file_handle <= 0) return false;
    else {
        this->size();
        return  true;
    }
#else
    file_handle = CreateFile(n.c_str(), a, 
                             NULL, NULL, o, 
                             FILE_FLAG_OVERLAPPED, NULL);
    
    if(file_handle == INVALID_HANDLE_VALUE) return false;
    else {
        this->size();
        return  true;
    }

#endif
}

void branch_io::file::close()
{
#ifdef UNIX_MODE
    close      (file_handle);
#else
    CloseHandle(file_handle);
#endif
}


size_t branch_io::file::read (uint8_t* c, size_t s)
{
    
}

size_t branch_io::file::write(uint8_t* c, size_t s)
{
    
}
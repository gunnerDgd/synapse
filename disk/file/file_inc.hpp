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

namespace synapse
{
namespace disk
{
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
            read_only    = GENERIC_READ,
            write_only   = GENERIC_WRITE,
            all          = GENERIC_ALL
      };
      using error_code = DWORD;
#endif
}
}
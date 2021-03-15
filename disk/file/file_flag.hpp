#pragma once
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
      using handle_t = int;
      enum access_mode : int
      {
            read_only    = O_RDONLY,
            write_only   = O_WRONLY,
            all          = O_RDWR
      };
      
      // Not valid in UNIX System.
      enum open_mode   : int
      {
            create,
            open_existing
      };

      using error_code = int;
#else
      using handle_t = HANDLE;
      enum access_mode
      {
            read_only    = GENERIC_READ,
            write_only   = GENERIC_WRITE,
            all          = GENERIC_ALL
      };

      enum open_mode
      {
            create        = CREATE_ALWAYS,
            open_existing = OPEN_ALWAYS
      };

      using error_code = DWORD;
#endif
}
}
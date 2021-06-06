#pragma once
#include <synapse/synapse.hpp>

#ifdef ENV_UNIX
#include <sys/types.h> 
#include <sys/stat.h>

#include <fcntl.h>
#include <unistd.h>

#endif

namespace synapse {
namespace device  {
#ifdef ENV_UNIX
      enum access_mode : int
      {
            read_only    = O_RDONLY,
            write_only   = O_WRONLY,
            all          = O_RDWR
      };

      int operator| (const access_mode& lhs, const access_mode& rhs) { return (int)lhs | (int)rhs; }
      int operator| (const access_mode& lhs, const int& rhs)         { return (int)lhs | rhs;      }
      
      // Not valid in UNIX System.
      enum open_mode   : int
      {
            create   = O_CREAT | O_EXCL,
            open     = O_CREAT         ,
            truncate = O_TRUNC
      };
      
      int operator| (const open_mode& lhs, const open_mode& rhs) { return (int)lhs | (int)rhs; }
      int operator| (const open_mode& lhs, const int& rhs)       { return (int)lhs | rhs;      }

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
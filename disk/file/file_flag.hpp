#include <synapse/synapse.hpp>

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
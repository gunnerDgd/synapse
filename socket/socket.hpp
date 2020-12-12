#include <synapse/stream/stream.hpp>

namespace network
{
          class socket : public stream::stream
          {
                    public:
                              socket  ();
                              ~socket();

                              virtual size_t send(uint8_t* s_ctx, size_t s_size) = 0;
                              virtual size_t recv(uint8_t* r_ctx, size_t r_size)  = 0;

                              size_t read (uint8_t* r_ctx, size_t r_size)    override { return recv(r_ctx, r_size);    }
                              size_t write(uint8_t* w_ctx, size_t w_size)  override { return send(w_ctx, w_size); }

          };

#ifdef UNIX_MODE
          using socket_type = int;
#endif

#ifdef WIN32_MODE
          using socket_type = SOCKET;
#endif
}
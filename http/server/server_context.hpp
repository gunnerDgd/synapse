#include <synapse/http/packet.hpp>
#include <synapse/thread/pool/thread_pool.hpp>

namespace http
{
    class client_handler : public pooled_launcher
    {
        public:
            void launch() override;

        private:
            
    };
}
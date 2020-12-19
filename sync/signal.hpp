#include <synapse/sync/event.hpp>

namespace synchronous
{
    template <typename T>
    class signal
    {
        public:
            void operator= (T _d) { signal_ctx = _d;   signal_evt.alert(); }
            T&   wait      ()     { signal_evt.wait(); return signal_ctx;  }

        private:
            T                  signal_ctx;
            synchronous::event signal_evt;
    };
}
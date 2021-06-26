#include <synapse/synapse.hpp>
#include <xmmintrin.h>

namespace synapse {
namespace system  {
namespace cpu     {

    template <typename T>
    inline void cache_prefetch(T& pref_ctx) { _mm_prefetch(&pref_ctx, _MM_HINT_T0); }
    inline void pause_cpu     ()            { _mm_pause(); }
}
}
}
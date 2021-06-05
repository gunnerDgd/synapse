#include <synapse/synapse.hpp>

namespace synapse {
namespace simd    {
    
    class sse_integral
    {
    public:
        sse_integral(__m128i& sse_init);

        sse_integral()                 { }

        void load   (void* );
        void store  (__m128i* sse_ctx);

    private:
        __m128i sse_integral_context;
    };

    friend sse_integral& operator+ (sse_integral& l, sse_integral& r);
    friend sse_integral& operator- (sse_integral& l, sse_integral& r);
    friend sse_integral& operator* (sse_integral& l, sse_integral& r);
    friend sse_integral& operator/ (sse_integral& l, sse_integral& r);
}
}

synapse::simd::sse_integral::sse_integral(__m128i* sse_init)
    : sse_integral_context(_mm_load_si128(sse_init)) { }
 
void synapse::simd::sse_integral::load (__m128i* sse_ctx) { sse_integral_context = _mm_load_si128(sse_ctx); }
void synapse::simd::sse_integral::store(__m128i* sse_ctx) { _mm_store_si128(sse_ctx, sse_integral_context); }

sse_integral& synapse::simd::operator+ (synapse::simd::sse_integral& l, synapse::simd::sse_integral& r)
{
    
}
sse_integral& synapse::simd::operator- (synapse::simd::sse_integral& l, synapse::simd::sse_integral& r);
sse_integral& synapse::simd::operator* (synapse::simd::sse_integral& l, synapse::simd::sse_integral& r);
sse_integral& synapse::simd::operator/ (synapse::simd::sse_integral& l, synapse::simd::sse_integral& r);
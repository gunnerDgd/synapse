#pragma once
#include <iostream>
#include <memory>
#include <synapse/synapse.hpp>


#if   SYNAPSE_SIMD_VER == 1
#include <mmintrin.h>
#elif SYNAPSE_SIMD_VER == 2
#include <xmmintrin.h>
#elif SYNAPSE_SIMD_VER == 3
#include <emmintrin.h>
#elif SYNAPSE_SIMD_VER == 4
#include <pmmintrin.h>
#elif SYNAPSE_SIMD_VER == 5
#include <tmmintrin.h>
#elif SYNAPSE_SIMD_VER == 5
#include <smmintrin.h>
#endif

#define CPU_CACHE_LINE 64

namespace simd
{	
	template <size_t N>
	struct simd_memory   
	{
		union
		{
			alignas(16) uint8_t    simd_mem_ctx[N * 16] = { 0x00, };
			__m128i				   simd_reg_ctx[N];
		};
	};
	struct simd_register { __m128i simd_reg_ctx[4];		 }; // Have 16 Byte *4 = 64 Byte
	
	template <size_t D, size_t S>
	void   copy_memory   (simd::simd_memory<D>& dst, simd::simd_memory<S>& src);
}

template <size_t D, size_t S>
void   simd::copy_memory(simd::simd_memory<D>& dst, simd::simd_memory<S>& src)
{
	size_t	  			copy_size = (D > S) ? S : D,
						copy_it = 0;

	simd::simd_register simd_reg;

	for ( ; copy_it < copy_size; copy_it += 4)
	{
		if (copy_size - copy_it > 4)
		{
			_mm_prefetch							 ((const char*)(src.simd_mem_ctx + copy_it + 1024), _MM_HINT_NTA);

			simd_reg.simd_reg_ctx[0] = _mm_load_si128((const __m128i*)src.simd_reg_ctx + copy_it);
			simd_reg.simd_reg_ctx[1] = _mm_load_si128((const __m128i*)src.simd_reg_ctx + copy_it + 1);
			simd_reg.simd_reg_ctx[2] = _mm_load_si128((const __m128i*)src.simd_reg_ctx + copy_it + 2);
			simd_reg.simd_reg_ctx[3] = _mm_load_si128((const __m128i*)src.simd_reg_ctx + copy_it + 3);

			_mm_store_si128(dst.simd_reg_ctx + copy_it,     simd_reg.simd_reg_ctx[0]);
			_mm_store_si128(dst.simd_reg_ctx + copy_it + 1, simd_reg.simd_reg_ctx[1]);
			_mm_store_si128(dst.simd_reg_ctx + copy_it + 2, simd_reg.simd_reg_ctx[2]);
			_mm_store_si128(dst.simd_reg_ctx + copy_it + 3, simd_reg.simd_reg_ctx[3]);
		}
		else
		{
			for (int i = 0; i < (copy_size - copy_it); i++)
			{
				simd_reg.simd_reg_ctx[0] = _mm_load_si128((const __m128i*)src.simd_reg_ctx + copy_it + i);
				_mm_store_si128(dst.simd_reg_ctx + copy_it + i, simd_reg.simd_reg_ctx[0]);
			}
		}
	}
}
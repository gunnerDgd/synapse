#pragma once
#include <iostream>
#include <memory>
#include <synapse/synapse.hpp>

namespace simd
{
	template <size_t N> // 16 * N
	struct simd_memory { alignas(16) uint8_t _ctx[N * 16]; };

	template <size_t _src, size_t _dst>
	void   copy(simd_memory<_src>& _s, simd_memory<_dst>& _d)
	{

	}
}
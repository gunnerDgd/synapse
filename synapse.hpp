#pragma once
#include <synapse/debug_tool/runtime_timer.hpp>

#define ENV_UNIX
//#define ENV_WINDOWS

#define SYNAPSE_VER 1.0.0
#define CPPVER 17

// 0 : Do Not Use SIMD
// 1 : SSE
// 2 : SSE2
// 3 : SSE3
// 4 : SSSE3
// 5 : SSE4
// 6 : AVX

#define SYNAPSE_SIMD_VER 2

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

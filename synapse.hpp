#pragma once

#define UNIX_MODE
//#define WIN32_MODE

#define SYNAPSE_VER 1.0.0
#define CPPVER 14

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

namespace synapse {}
using namespace synapse;
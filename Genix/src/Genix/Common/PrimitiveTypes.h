#pragma once
#include <cstdint>

// unsigned integers
using uint64 = uint64_t;
using uint32 = uint32_t;
using uint16 = uint16_t;
using uint8	 = uint8_t;

// signed integers
using int64 = int64_t;
using int32 = int32_t;
using int16 = int16_t;
using int8 = int8_t;

enum class BlendFuncArgument
{
	OneMinusSrcAlpha = 0,
	SrcAlpha = 1
};

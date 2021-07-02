#pragma once

#include <math.h>

namespace RE::Math
{
#define RE_INDEX uint16_t

	inline float degToRad( float deg )
	{
		return (deg * M_PI) / 180.0f;
	}

	inline float radToDeg( float rad )
	{
		return (rad * 180.0f) / M_PI;
	}

	template<typename T> inline T Align( T val, size_t alignment )
	{
		size_t mask = alignment - 1;
		return (T)(((size_t)val + mask) & ~mask);
	}
}

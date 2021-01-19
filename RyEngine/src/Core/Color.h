#pragma once

#include "CoreMath.h"

#define RE_WHITE ::RE::Color{1.0f, 1.0f, 1.0f, 1.0f}
#define RE_BLACK ::RE::Color{0.0f, 0.0f, 0.0f, 1.0f}
#define RE_RED ::RE::Color{1.0f, 0.0f, 0.0f, 1.0f}
#define RE_GREEN ::RE::Color{0.0f, 1.0f, 0.0f, 1.0f}
#define RE_BLUE ::RE::Color{0.0f, 0.0f, 1.0f, 1.0f}
#define RE_YELLOW ::RE::Color{1.0f, 1.0f, 0.0f, 1.0f}
#define RE_CYAN ::RE::Color{0.0f, 1.0f, 1.0f, 1.0f}
#define RE_MAGENTA ::RE::Color{1.0f, 0.0f, 1.0f, 1.0f}

namespace RE
{
	union Color
	{
		struct c
		{
			FLOAT r, g, b, a;
		} c;
		FLOAT rbga[4];
	};

	inline Vector4 ColorToVector(Color c)
	{
		Vector4 ret{};
		ret.x = c.rbga[0];
		ret.y = c.rbga[1];
		ret.z = c.rbga[2];
		ret.w = c.rbga[3];

		return ret;
	}
}
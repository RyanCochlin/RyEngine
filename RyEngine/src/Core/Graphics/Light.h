#pragma once

#include "Core/Math/Vector.h"

namespace RE
{
    enum LightType
    {
        Directional
    };

	struct Light
	{
        LightType Type;
        Vector3 Strength;
        Vector3 Direction;
        Vector3 Position;
        float FalloffStart;
        float FalloffEnd;
        float SpotPower;
	};
}

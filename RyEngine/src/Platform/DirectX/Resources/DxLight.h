#pragma once

#include "Core/Graphics/Light.h"

namespace RE
{
	struct DxLight
	{
		DxLight() = default;
		DxLight(Light* light)
		{
			Strength = light->Strength;
			FalloffStart = light->FalloffStart;
			FalloffEnd = light->FalloffEnd;
			Direction = light->Direction;
			Position = light->Position;
			SpotPower = light->SpotPower;
		}

		Vector3 Strength;
		float FalloffStart;
		Vector3 Direction;
		float FalloffEnd;
		Vector3 Position;
		float SpotPower;
	};
}
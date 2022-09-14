#pragma once

#include "Core/Math/Vector.h"

namespace RE
{
	struct MaterialData
	{
		Vector4 diffuse;
		Vector3 fresnel;
		float rough;
	};

	struct Material
	{
		const char* name;
		MaterialData materialData;
	};
}
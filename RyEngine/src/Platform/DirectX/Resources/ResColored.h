#pragma once
#include "Core/Math/Matrix.h"
#include "DxLight.h"

namespace RE
{
#define MAX_LIGHTS 4

	struct ResColoredPassConstants
	{
		Matrix4 ViewProj;
		Vector4 Ambient;
		Vector3 EyePosition;
		uint32_t DirLightCount;
		uint32_t PointLightCount;
		uint32_t SpotLightCount;
		uint32_t pad1 = 0;
		uint32_t pad2 = 0;

		DxLight lights[MAX_LIGHTS];
	};

	struct ResColoredObjectConstants
	{
		Matrix4 World;
	};

	struct ResColoredMaterialConstants
	{
		Vector4 DiffuseAlbedo;
		Vector3 FresnelR0;
		float  Roughness;
	};
}

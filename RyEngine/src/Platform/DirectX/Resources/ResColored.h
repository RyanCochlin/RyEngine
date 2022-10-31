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

#pragma once
#include "Core/Math/Matrix.h"
#include "UploadResource.h"

namespace RE
{
	struct ResColoredPassConstants
	{
		Matrix4 ViewProj;
		Vector4 Ambient;
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

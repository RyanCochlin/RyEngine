#pragma once
#include "Core/Math/Matrix.h"
#include "UploadResource.h"

namespace RE
{
	struct ResColoredPassConstants
	{
		Matrix4 ViewProj;
	};

	struct ResColoredObjectConstants
	{
		Matrix4 World;
	};
}

#pragma once

#include "Core/Math/Vector.h"
#include "Core/Math/Matrix.h"

namespace RE
{
	class Transform
	{
	public:
		RE_API Transform();
		RE_API ~Transform() = default;

		RE_API Vector3 GetTranslation();
		RE_API Vector3 GetRotation();
		RE_API Vector3 GetScale();
		RE_API void Translate(Vector3 translation);
		RE_API void Rotate(Vector3 rotation);
		RE_API void Scale(Vector3 scale);
		RE_API Matrix4 GetWorld();

	private:
		Matrix4 _mTRSMatrix;
		Vector3 _mTranslation;
		Vector3 _mRotation;
		Vector3 _mScale;
	};
}


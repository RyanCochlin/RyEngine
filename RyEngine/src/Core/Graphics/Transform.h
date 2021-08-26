#pragma once

#include "Core/Math/Vector.h"
#include "Core/Math/Matrix.h"

namespace RE
{
	class Transform
	{
	public:
		Transform();
		~Transform() = default;

		Vector3 GetTranslation();
		Vector3 GetRotation();
		Vector3 GetScale();
		void Translate(Vector3 translation);
		void Rotate(Vector3 rotation);
		void Scale(Vector3 scale);
		Matrix4 GetWorld();

	private:
		Matrix4 _mTRSMatrix;
		Vector3 _mTranslation;
		Vector3 _mRotation;
		Vector3 _mScale;
	};
}


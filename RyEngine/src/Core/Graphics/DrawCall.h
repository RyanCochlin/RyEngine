#pragma once
#include "Core/Math/Matrix.h"

namespace RE
{
	class DrawCall
	{
	public:
		DrawCall();

		void SetMVP(Matrix4 mvp);
		Matrix4 GetMVP();

		void SetAmbient(Vector4 ambient);
		Vector4 GetAmbient();

	protected:
		Matrix4 _mModelViewProjection;
		Vector4 _mAmbientLight;
	};
}

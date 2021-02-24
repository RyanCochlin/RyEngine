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

	protected:
		Matrix4 _mModelViewProjection;
	};
}

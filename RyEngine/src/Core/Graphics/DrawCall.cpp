#include "pch.h"
#include "DrawCall.h"

namespace RE
{
	DrawCall::DrawCall() :
		_mModelViewProjection{}
	{}

	void DrawCall::SetMVP(Matrix4 mvp)
	{
		_mModelViewProjection = mvp;
	}

	Matrix4 DrawCall::GetMVP()
	{
		return _mModelViewProjection;
	}
}
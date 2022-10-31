#include "pch.h"
#include "DrawCall.h"

namespace RE
{
	DrawCall::DrawCall() :
		_mModelViewProjection{},
		_mLights()
	{}

	void DrawCall::SetMVP(Matrix4& mvp)
	{
		_mModelViewProjection = mvp;
	}

	Matrix4 DrawCall::GetMVP()
	{
		return _mModelViewProjection;
	}

	void DrawCall::SetAmbient(Vector4& ambient)
	{
		_mAmbientLight = ambient;
	}

	Vector4 DrawCall::GetAmbient()
	{
		return _mAmbientLight;
	}

	void DrawCall::SetEyePosition(Vector3& position)
	{
		_mEyePosition = position;
	}

	Vector3 DrawCall::GetEyePosition()
	{
		return _mEyePosition;
	}

	void DrawCall::AddLight(Light* light)
	{
		_mLights.push_back(light);
	}

	std::vector<Light*>& DrawCall::GetLights()
	{
		return _mLights;
	}
}